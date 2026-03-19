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

VideoDecoder::VideoDecoder(const std::string & path) {
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

    packet = av_packet_alloc();
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    swsContext = sws_getContext(codecContext->width, codecContext->height,
    codecContext->pix_fmt, 170, 80, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr,
nullptr, nullptr);

int bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 170, 80, 1);
buffer = (uint8_t *) av_malloc(bytes);
av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
AV_PIX_FMT_RGB24, 170, 80, 1);
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
        av_packet_unref(packet);
        return true;
    }

    av_packet_unref(packet);
}

return false;
}



void VideoDecoder::renderVideo(void) {
    auto getPixel = [&] (int x, int y) {
        int offset = y * rgbFrame->linesize[0] + x * 3;

        uint8_t tr = rgbFrame->data[0][offset];
        uint8_t tg = rgbFrame->data[0][offset + 1];
        uint8_t tb = rgbFrame->data[0][offset + 2];

        return RGB(tr, tg, tb);
    };

RGB prevBottom;
RGB prevTop;

while(getNextFrame()) {

std::stringstream ss;
ss << "\033[H";
std::cout << "\033[H";
std::cout << "\033[?25l";

for(int y = 0; y < 80; y+=2) {
    for(int x = 0; x < 170; x++) {

        RGB topPixel = getPixel(x, y);
        RGB bottomPixel = (y + 1 < 80) ? getPixel(x, y + 1) : RGB();

        topPixel.printPixel(ss, bottomPixel, prevTop, prevBottom);

    }
     ss << "\033[0m\n";
}

std::cout << ss.str() << std::fflush;

std::this_thread::sleep_for(std::chrono::milliseconds(33));
}

std::cout << "\033[?25h";
}