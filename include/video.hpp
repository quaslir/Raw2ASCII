#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "fps.hpp"
#include "rgb.hpp"
#include "utils.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>
#include "audio.hpp"
struct Frame {
  std::string data;
  double duration;

  Frame(std::string &&s, double dur) : data(std::move(s)), duration(dur) {}
  Frame() = default;
};

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
  FPS fps;
  std::queue<Frame> readyData;
  AudioPlayer audio;
  std::unique_ptr<RGB[]> getReadyFrame(void);
  std::string renderStream(RGB *currentFrame) const;
  void fillQueue(void);
  bool getNextFrame(void);

public:
  VideoDecoder(const utils::Options &options);

  void renderVideo(void);
  ~VideoDecoder();
};