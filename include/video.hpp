#pragma once

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>

}

#include <string>
#include <stdexcept>
#include <vector>
#include <stdint.h>
#include "rgb.hpp"
class VideoDecoder {
private:

AVFormatContext * formatContext = nullptr;
AVCodecContext * codecContext = nullptr;
int videoStreamIndex = -1;
AVPacket * packet = nullptr;
AVFrame * frame = nullptr;
AVFrame *rgbFrame = nullptr;
SwsContext * swsContext = nullptr;
uint8_t * buffer = nullptr;

public:
VideoDecoder(const std::string & path);
bool getNextFrame(void);
void renderVideo(void);
~VideoDecoder();
};