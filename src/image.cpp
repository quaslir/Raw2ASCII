#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"
#include "stb_image.h"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
Image::Image(const utils::Options &options) : data(nullptr, stbi_image_free) {

  unsigned char *raw =
      stbi_load(options.file.c_str(), &width, &height, &channels, 4);
  try {
    processRaw(raw);
    opts = options;

  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
  }
}

Image::Image(const utils::Options &options, std::string &&buffer)
    : data(nullptr, stbi_image_free) {
  unsigned char *raw = stbi_load_from_memory(
      reinterpret_cast<const stbi_uc *>(buffer.data()),
      static_cast<int>(buffer.size()), &width, &height, &channels, 4);
  try {
    processRaw(raw);
    opts = options;

  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
  }
}

void Image::processRaw(unsigned char *raw) {
  if (!raw) {
    throw std::runtime_error("Failed to load image");
  }

  data.reset(reinterpret_cast<RGB *>(raw));
}

void Image::renderImage(void) const {
  if (!data)
    return;

  int stepX = width / opts.targetWidth;
  int stepY = height / opts.targetHeight / 2;

  std::string buffer;

  if (opts.braille) {

  } else {

    for (int y = 0; y < height; y += stepY * 2) {
      RGB prevTop;
      RGB prevBottom;
      prevTop.alpha = 123;
      prevBottom.alpha = 123;
      for (int x = 0; x < width; x += stepX) {

        const RGB &top = data[y * width + x];
        int bottomIdx = (y + stepY < height) ? (y + stepY) : y;
        const RGB &bottom = data[(bottomIdx)*width + x];
        top.printPixel(buffer, bottom, prevTop, prevBottom, opts.threshold);
      }
      buffer += "\x1b[0m\n";
    }
  }

  if (opts.outputPath.empty())
    std::cout << buffer;
    std::stringstream ss(buffer);
    saveToFile(ss);

}

void Image::saveToFile(std::stringstream &ss) const {
  std::ofstream file(opts.outputPath);

  file << ss.str();
}