#include "gif.hpp"
#include "stb_image.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

Gif::Gif(std::vector<char> &&buffer, const utils::Options &options) {
  int w, h, count, channels;
  int *delays = nullptr;

  unsigned char *raw = stbi_load_gif_from_memory(
      reinterpret_cast<const stbi_uc *>(buffer.data()),
      static_cast<int>(buffer.size()), &delays, &w, &h, &count, &channels, 4);

  if (!raw) {
    throw std::runtime_error("Failed to load GIF");
  }

  width = w;
  height = h;

  size_t frameSize = width * height * 4;

  data.resize(count);

  for (int i = 0; i < count; i++) {
    const unsigned char *framePtr = raw + (i * frameSize);

    data[i].delay = delays[i];
    data[i].frame.resize(width * height);
    std::memcpy(data[i].frame.data(), framePtr, frameSize);
  }
  stbi_image_free(raw);

  stbi_image_free(delays);

  opts = options;
};

void Gif::renderGif(void) const {

  int stepX = width / opts.targetWidth;
  int stepY = height / opts.targetHeight;

  std::cout << "\033[2J\033[?25l";

  for (size_t i = 0; i < data.size(); i++) {
    std::string buffer = "\033[H";
    if(opts.braille) {
      buffer += opts.renderBraille(data[i].frame);
    }
    else {
    for (int y = 0; y < height; y += stepY * 2) {
      RGB prevTop(0, 0, 0);
      RGB prevBottom(0, 0, 0);
      prevTop.alpha = 0;
      prevBottom.alpha = 0;
      for (int x = 0; x < width; x += stepX) {

        const RGB &top = data[i].frame[y * width + x];
        int bottomIdx = (y + stepY < height) ? (y + stepY) : y;
        const RGB &bottom = data[i].frame[(bottomIdx)*width + x];

        top.printPixel(buffer, bottom, prevTop, prevBottom, opts.threshold);
      }
      buffer += "\033[0m\n";
    }
  }
    if (opts.outputPath.empty()) {

      std::cout << buffer;
      std::this_thread::sleep_for(std::chrono::milliseconds(data[i].delay));
    } else {
      opts.writeFile(std::move(buffer));
    }
  }

  std::cout << "\033[?25h" << std::endl;
}
