#include "video.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
VideoDecoder::~VideoDecoder() {

  av_packet_free(&packet);
  av_frame_free(&frame);
  av_frame_free(&rgbFrame);
  avcodec_free_context(&codecContext);

  avformat_close_input(&formatContext);
  av_free(buffer);
  sws_freeContext(swsContext);
}

VideoDecoder::VideoDecoder(const utils::Options &options) {
  if (avformat_open_input(&formatContext, options.file.c_str(), nullptr,
                          nullptr) < 0) {
    throw std::runtime_error("could not open file ");
  }

  if (avformat_find_stream_info(formatContext, nullptr) < 0) {
    throw std::runtime_error("could not find stream data");
  }

  const AVCodec *videoCodec = nullptr;
  videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1,
                                         -1, &videoCodec, 0);

  if (videoStreamIndex < 0) {
    throw std::runtime_error("video data could not be found");
  }
  codecContext = avcodec_alloc_context3(videoCodec);
  if (avcodec_parameters_to_context(
          codecContext, formatContext->streams[videoStreamIndex]->codecpar) <
      0) {
    throw std::runtime_error("could not copy codec params");
  }

  if (avcodec_open2(codecContext, videoCodec, nullptr) < 0) {
    throw std::runtime_error("could not open codec");
  }

  audio.getFormatContext = [this]() { return formatContext; };

  opts = options;
  if(opts.outputPath.empty()) {
  audio.init();
  }
  packet = av_packet_alloc();
  frame = av_frame_alloc();
  rgbFrame = av_frame_alloc();
  swsContext =
      sws_getContext(codecContext->width, codecContext->height,
                     codecContext->pix_fmt, opts.targetWidth, opts.targetHeight,
                     AV_PIX_FMT_RGBA, SWS_BILINEAR, nullptr, nullptr, nullptr);

  int bytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, opts.targetWidth,
                                       opts.targetHeight, 1);
  buffer = static_cast<uint8_t *>(av_malloc(bytes));
  av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
                       AV_PIX_FMT_RGBA, opts.targetWidth, opts.targetHeight, 1);
}

bool VideoDecoder::getNextFrame(void) {
  while (av_read_frame(formatContext, packet) >= 0) {
    if (packet->stream_index == videoStreamIndex) {

      int response = avcodec_send_packet(codecContext, packet);

      if (response < 0) {
        av_packet_unref(packet);
        return false;
      }

      response = avcodec_receive_frame(codecContext, frame);

      if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
        av_packet_unref(packet);
        continue;
      } else if (response < 0) {
        av_packet_unref(packet);
        return false;
      }

      double pts = 0;

      if (frame->best_effort_timestamp != AV_NOPTS_VALUE) {
        AVRational tb = formatContext->streams[videoStreamIndex]->time_base;
        pts = frame->best_effort_timestamp * av_q2d(tb);
      }

      sws_scale(swsContext, frame->data, frame->linesize, 0,
                codecContext->height, rgbFrame->data, rgbFrame->linesize);
      std::unique_ptr<RGB[]> readyFrame = getReadyFrame();
      std::string toASCII = renderStream(readyFrame.get());
      {
        std::lock_guard<std::mutex> lock(queueMutex);
        readyData.push(Frame(std::move(toASCII), pts));
      }

      av_packet_unref(packet);
      return true;
    } else if (packet->stream_index == audio.frameIndex) {
      audio.decode(packet);
    }

    av_packet_unref(packet);
  }

  return false;
}

std::unique_ptr<RGB[]> VideoDecoder::getReadyFrame(void) {
  size_t size = opts.targetWidth * opts.targetHeight;
  std::unique_ptr<RGB[]> data = std::make_unique<RGB[]>(size);

  uint8_t *dst = reinterpret_cast<uint8_t *>(data.get());
  uint8_t *src = rgbFrame->data[0];

  for (int y = 0; y < opts.targetHeight; y++) {
    std::memcpy(dst, src, opts.targetWidth * 4);

    src += rgbFrame->linesize[0];

    dst += opts.targetWidth * 4;
  }

  return data;
}

std::string VideoDecoder::renderStream(const RGB *currentFrame) const {

  std::string currentFrameBuffer;
  currentFrameBuffer.reserve(opts.targetHeight * opts.targetWidth * 20);

  for (int y = 0; y < opts.targetHeight; y += 2) {
    RGB prevBottom;
    RGB prevTop;
    for (int x = 0; x < opts.targetWidth; x++) {

      const RGB &top = currentFrame[y * opts.targetWidth + x];
      int bottomIdx = (y + 1 < opts.targetHeight) ? (y + 1) : y;
      const RGB &bottom = currentFrame[(bottomIdx)*opts.targetWidth + x];

      top.printPixel(currentFrameBuffer, bottom, prevTop, prevBottom, opts.threshold);
    }
    currentFrameBuffer += "\033[0m\n";
  }

  return currentFrameBuffer;
}

void VideoDecoder::renderVideo(void) {
  std::thread decoder([&]() {
    fillQueue();
    isDecodingFinished = true;
  });

  auto startTime = std::chrono::steady_clock::now();
  if (opts.outputPath.empty()) {
    std::cout << "\033[2J\033[?25l";
  } else {
    opts.writeFile("\033[2J\033[?25l");
  }

  while (true) {

    Frame currentFrame;
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      if (!readyData.empty()) {
        currentFrame = std::move(readyData.front());
        readyData.pop();
      }
    }

    if (currentFrame.data.empty()) {
      if (isDecodingFinished)
        break;

      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      continue;
    }

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - startTime;
    double delay = currentFrame.duration - elapsed.count();

    if (delay > 0.005) {
      std::this_thread::sleep_for(std::chrono::duration<double>(delay));
    } else if (delay < -0.05) {
      continue;
    }

    if (opts.outputPath.empty()) {
      std::fwrite(currentFrame.data.c_str(), 1, currentFrame.data.size(),
                  stdout);
      std::fwrite("\033[H", 1, 3, stdout);
      if (opts.fps) {
        fps.update();
        std::string fpsStr = fps.display() + "\033[K\n";
        std::fwrite(fpsStr.data(), 1, fpsStr.size(), stdout);
      }
    } else {
      opts.writeFile(std::move(currentFrame.data));
      opts.writeFile("\033[H");
    }
  }

  if (decoder.joinable())
    decoder.join();

  if (opts.outputPath.empty()) {
    std::cout << "\033[?25h";
  } else {
    opts.writeFile("\033[?25h");
  }
}

void VideoDecoder::fillQueue(void) {
  while (getNextFrame()) {
    while (readyData.size() > 100) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}
