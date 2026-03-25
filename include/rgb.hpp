#include <sstream>
#include <stdint.h>
#pragma once
#pragma pack(push, 1)
struct RGB {
  uint8_t r, g, b, alpha;

  void printPixel(std::string &str, const RGB &bottom, RGB &prevTop,
                  RGB &prevBottom, int th = 0) const;
  bool operator==(const RGB &other) const;
  bool operator!=(const RGB &other) const;
  RGB() : r(0), g(0), b(0), alpha(0) {}
  RGB(uint8_t red, uint8_t green, uint8_t blue)
      : r(red), g(green), b(blue), alpha(255) {}
};
#pragma pack(pop)
