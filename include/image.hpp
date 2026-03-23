#pragma once
#include "rgb.hpp"
#include "utils.hpp"
#include <memory>
#include <string>
extern "C" void stbi_image_free(void *);
class Image {
  using StbPtr = std::unique_ptr<RGB[], decltype(&stbi_image_free)>;

private:
  int width, height, channels;
  StbPtr data;
  utils::Options opts;

  void saveToFile(std::stringstream &ss) const;

public:
  Image(const utils::Options &options);
  void renderImage(void) const;
};