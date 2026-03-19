#include "video.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
VideoDecoder::~VideoDecoder() {
    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

VideoDecoder::VideoDecoder(const std::string & path, const utils::Options & options) {
    if(avformat_open_input(&formatContext, path.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("could not open file " + path);
    }

    if(avformat_find_stream_info(formatContext, nullptr) < 0) {
        throw std::runtime_error("could not find stream data");
    }

    const AVCodec * codec = nullptr;
    videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);

    if(videoStreamIndex < 0) {
        throw std::runtime_error("video data could not be found");
    }
    codecContext = avcodec_alloc_context3(codec);
    if(avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        throw std::runtime_error("could not copy codec params");
    }

    if(avcodec_open2(codecContext, codec, nullptr) < 0) {
        throw std::runtime_error("could not open codec");
    }
    opts = options;

    packet = av_packet_alloc();
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    swsContext = sws_getContext(codecContext->width, codecContext->height,
    codecContext->pix_fmt, opts.targetWidth, opts.targetHeight, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr,
nullptr, nullptr);

int bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, opts.targetWidth, opts.targetHeight, 1);
buffer = (uint8_t *) av_malloc(bytes);
av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
AV_PIX_FMT_RGB24, opts.targetWidth, opts.targetHeight, 1);


}


bool VideoDecoder::getNextFrame(void) {
while(av_read_frame(formatContext, packet) >= 0) {
    if(packet->stream_index == videoStreamIndex) {
        
        int response = avcodec_send_packet(codecContext, packet);

        if(response < 0) {
            av_packet_unref(packet);
            return false;
        }

        response = avcodec_receive_frame(codecContext, frame);

        if(response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(packet);
            continue;
        } else if(response < 0) {
            av_packet_unref(packet);
            return false;
        }

        sws_scale(
            swsContext,
            frame->data,
            frame->linesize,
            0,
            codecContext->height,
            rgbFrame->data,
            rgbFrame->linesize
        ); 
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            readyData.push(getReadyFrame());
        }
        
        av_packet_unref(packet);
        return true;
    }

    av_packet_unref(packet);
}

return false;
}


RGB VideoDecoder::getPixel(int x, int y) const {
        int offset = y * rgbFrame->linesize[0] + x * 3;

        uint8_t tr = rgbFrame->data[0][offset];
        uint8_t tg = rgbFrame->data[0][offset + 1];
        uint8_t tb = rgbFrame->data[0][offset + 2];

        return RGB(tr, tg, tb);
}


std::vector<RGB> VideoDecoder::getReadyFrame(void) {
    std::vector<RGB> data;
for(int y = 0; y < opts.targetHeight; y++) {
    for(int x = 0; x < opts.targetWidth; x++) {

        data.push_back(getPixel(x, y));
    }

}
return data;
}

void VideoDecoder::renderStream(const std::vector<RGB>& currentFrame) const {
RGB prevBottom;
        RGB prevTop;

        std::stringstream ss;
std::cout << "\033[H";



for(int y = 0; y < opts.targetHeight; y+=2) {
    for(int x = 0; x < opts.targetWidth; x++) {

        const RGB& top = currentFrame[y * opts.targetWidth + x];
        int bottomIdx = (y + 1 < opts.targetHeight) ? (y + 1) : y;
        const RGB& bottom = currentFrame[(bottomIdx) * opts.targetWidth + x];

        top.printPixel(ss, bottom, prevTop, prevBottom);

    }
     ss << "\033[0m\n";
}

std::cout << ss.str();

}


void VideoDecoder::renderVideo(void) {
    std::thread decoder([&]() {
        fillQueue();
        isDecodingFinished = true;
    });

        

const std::chrono::microseconds frameDur(16666);
std::cout << "\033[?25l";
while(true) {

auto startTime = std::chrono::high_resolution_clock::now();
std::vector<RGB> currentFrame;
{
    std::lock_guard<std::mutex> lock(queueMutex);
    if(!readyData.empty()) {
        currentFrame = std::move(readyData.front());
        readyData.pop();
    }
}


if(currentFrame.empty()) {
    if(isDecodingFinished) break;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    continue;
}

renderStream(currentFrame);
auto endTime = std::chrono::high_resolution_clock::now();

auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

if(elapsed < frameDur) {
std::this_thread::sleep_for(frameDur - elapsed);
}


}

if(decoder.joinable()) decoder.join();
std::cout << "\033[?25h";
}

void VideoDecoder::fillQueue(void) {
    while(getNextFrame()) {
        while(readyData.size() > 100) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}