#include "audio.hpp"
#include "miniaudio.h"
#include <cstring>
#include <stdexcept>
void AudioPlayer::init() {
   AVFormatContext * formatContext = getFormatContext();
  const AVCodec *audioCodec = nullptr;
  frameIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, 0);

  if(frameIndex >= 0) {
    codecContext = avcodec_alloc_context3(audioCodec);
    avcodec_parameters_to_context(codecContext, formatContext->streams[frameIndex]->codecpar);

    if(avcodec_open2(codecContext, audioCodec, nullptr) < 0) {
      throw std::runtime_error("could not open audio codec");
    }
  } else return; 

  av_channel_layout_default(&layout, 2);
  int res =  swr_alloc_set_opts2(&swrContext, &layout, AV_SAMPLE_FMT_FLT, 48000,
    &codecContext->ch_layout, codecContext->sample_fmt, codecContext->sample_rate, 0, nullptr);

    if(res < 0 || swr_init(swrContext) < 0) {
      throw std::runtime_error("could not initialize audio resampler");
    }
    audioFrame = av_frame_alloc();
  ma_device_config config = ma_device_config_init(ma_device_type_playback);

  config.playback.format = ma_format_f32;
  config.playback.channels = 2;
  config.sampleRate = 48000;
  config.dataCallback = dataCallback;
  config.pUserData = this;

  if (ma_device_init(NULL, &config, &dev) != MA_SUCCESS) {
    throw std::runtime_error("Failed to open playback device");
  }

 
  
  ma_result initRes = ma_pcm_rb_init(ma_format_f32, 2, 48000 * 10,NULL, NULL, &rb);

  if(initRes != MA_SUCCESS) {
    throw std::runtime_error("failed to init ring buffer");
  }

   if (ma_device_start(&dev) != MA_SUCCESS) {
    throw std::runtime_error("Failed to start playback device");
  }
}

AudioPlayer::~AudioPlayer() {
  swr_free(&swrContext);
  avcodec_free_context(&codecContext);
   ma_device_uninit(&dev); 
  ma_pcm_rb_uninit(&rb);
  av_frame_free(&audioFrame);
  }

void AudioPlayer::dataCallback(ma_device * pDev, void * output, const void *input, ma_uint32 frames) {
AudioPlayer *pAudio = static_cast<AudioPlayer*>(pDev->pUserData);

if(!pAudio) return;

size_t samples = frames * 2;
size_t bytes = samples * sizeof(float);
ma_uint32 framesRead = frames;


void *pBufferIn = nullptr;
float *pOut = static_cast<float*>(output);

ma_result res = ma_pcm_rb_acquire_read(&pAudio->rb, &framesRead, &pBufferIn);

if(res == MA_SUCCESS && framesRead > 0) {
std::memcpy(pOut, pBufferIn, framesRead * 2 * sizeof(float));
ma_pcm_rb_commit_read(&pAudio->rb, framesRead);

if(framesRead < frames) {
  ma_uint32 dif = frames - framesRead;
  float *pNextout = pOut + (framesRead * 2);

  void *pBufferInNext = nullptr;

  if(ma_pcm_rb_acquire_read(&pAudio->rb, &dif, &pBufferInNext) == MA_SUCCESS) {
    std::memcpy(pNextout, pBufferInNext, dif * 2 * sizeof(float));
    ma_pcm_rb_commit_read(&pAudio->rb, dif);
  }
}

} else {
  std::memset(output,0 , frames * 2 * sizeof(float));
}

}

void AudioPlayer::processAudioFrame(AVFrame * frame) {
int samples = av_rescale_rnd(swr_get_delay(swrContext,frame->sample_rate) + frame->nb_samples,
48000, frame->sample_rate, AV_ROUND_UP);

uint8_t * outputBuffer = nullptr;

av_samples_alloc(&outputBuffer, NULL, 2, samples, AV_SAMPLE_FMT_FLT, 0);

int converted = swr_convert(swrContext, &outputBuffer, samples, (const uint8_t **)frame->data, frame->nb_samples );

if(converted > 0) {
  pushData(reinterpret_cast<float*>(outputBuffer), converted);
}

av_freep(&outputBuffer);
}

void AudioPlayer::decode(AVPacket * packet) {
if(!packet) return;

int res = avcodec_send_packet(codecContext, packet);
if(res < 0) return;

while(res >= 0) {
  res = avcodec_receive_frame(codecContext, audioFrame);

  if(res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
    break;
  }
  else if(res < 0) break;

  processAudioFrame(audioFrame);
  av_frame_unref(audioFrame);
}
}

void AudioPlayer::pushData(const float * data, ma_uint32 count) {
if(!data || count == 0) return;

ma_uint32 framesToWrite = count;
void *bufferOut = nullptr;

ma_result res = ma_pcm_rb_acquire_write(&rb, &framesToWrite, &bufferOut);

if(res == MA_SUCCESS && framesToWrite > 0) {
  std::memcpy(bufferOut, data, framesToWrite * 2 * sizeof(float));
  ma_pcm_rb_commit_write(&rb, framesToWrite);

  if(framesToWrite < count) {
    ma_uint32 dif = count - framesToWrite;

    const float * remaining = data + (framesToWrite * 2);

    void *bufferNext = nullptr;

    if(ma_pcm_rb_acquire_write(&rb, &dif, &bufferNext) == MA_SUCCESS) {
      std::memcpy(bufferNext, remaining, dif * 2 * sizeof(float));
      ma_pcm_rb_commit_write(&rb, dif);
    }
  }
}
}