#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "rgb.hpp"
#include "utils.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>
class VideoDecoder {
private:
  AVFormatContext *formatContext = nullptr;
  AVCodecContext *codecContext = nullptr;
  int videoStreamIndex = -1;
  AVPacket *packet = nullptr;
  AVFrame *frame = nullptr;
  AVFrame *rgbFrame = nullptr;
  SwsContext *swsContext = nullptr;
  uint8_t *buffer = nullptr;
  utils::Options opts;
  std::mutex queueMutex;
  bool isDecodingFinished = false;

  std::queue<std::vector<RGB>> readyData;
  std::vector<RGB> getReadyFrame(void);
  RGB getPixel(int x, int y) const;
  void renderStream(const std::vector<RGB> &currentFrame) const;
  void fillQueue(void);

public:
  VideoDecoder(const std::string &path, const utils::Options &options);
  bool getNextFrame(void);
  void renderVideo(void);
  ~VideoDecoder();
};