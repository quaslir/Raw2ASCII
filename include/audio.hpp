#include <functional>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <vector>
extern "C" {
#include "miniaudio.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

struct AudioPlayer {

  AudioPlayer() {}
  ~AudioPlayer();

  void init(void);

  static void dataCallback(ma_device *pDev, void *output, const void *input,
                           ma_uint32 frames);

  void processAudioFrame(AVFrame *frame);
  void decode(AVPacket *packet);
  void pushData(const int16_t *data, ma_uint32 count);
  ma_device dev;
  ma_pcm_rb rb;
  SwrContext *swrContext = nullptr;
  AVCodecContext *codecContext = nullptr;
  int frameIndex = -1;
  AVFrame *audioFrame = nullptr;
  AVChannelLayout layout;
    std::vector<uint8_t> buffer;
  std::function<AVFormatContext *(void)> getFormatContext;
};