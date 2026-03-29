#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"
#include "color8bit.hpp"
#include "stb_image.h"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
Image::Image(utils::Options &&options) : data(nullptr, stbi_image_free) {

  unsigned char *raw =
      stbi_load(options.file.c_str(), &width, &height, &channels, 4);
  try {
    processRaw(raw);
    opts = std::move(options);

  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
  }
}

Image::Image(utils::Options &&options, std::string &&buffer)
    : data(nullptr, stbi_image_free) {
  unsigned char *raw = stbi_load_from_memory(
      reinterpret_cast<const stbi_uc *>(buffer.data()),
      static_cast<int>(buffer.size()), &width, &height, &channels, 4);
  try {
    processRaw(raw);
    opts = std::move(options);

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

  for (int y = 0; y < height; y += stepY * 2) {
    RGB prevTop(255, 255, 255, 255);
    RGB prevBottom(255, 255, 255, 255);
    for (int x = 0; x < width; x += stepX) {
      const RGB &top = data[y * width + x];
      int bottomIdx = (y + stepY < height) ? (y + stepY) : y;
      const RGB &bottom = data[(bottomIdx)*width + x];
      if (!opts.eight_bit_mode) {
        top.printPixel(buffer, bottom, prevTop, prevBottom, opts.threshold);
      } else {

        eight_bit::printPixel(buffer, top, bottom, prevTop, prevBottom);
      }
    }
    buffer += "\033[0m\n";
  }

  if (opts.outputPath.empty())
    std::cout << buffer;
  opts.writeFile(std::move(buffer));
}
