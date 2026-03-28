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

  void processRaw(unsigned char *raw);

public:
  explicit Image(utils::Options &&options);
  Image(utils::Options &&options, std::string &&buffer);
  void renderImage(void) const;
};