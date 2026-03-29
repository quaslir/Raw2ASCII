#include "color8bit.hpp"
#include <stdint.h>
#include <string>

namespace eight_bit {
uint8_t to_eight_bit(const RGB &pixel) {
  return 16 + (pixel.r * 6 / 256) * 36 + (pixel.g * 6 / 256) +
         (pixel.b * 6 / 256);
}

void printPixel(std::string &str, const RGB &top, const RGB &bottom,
                RGB &prevTop, RGB &prevBottom, int th) {
  uint8_t topColor = to_eight_bit(top);
  uint8_t bottomColor = to_eight_bit(bottom);
  uint8_t prevTopColor = to_eight_bit(prevTop);
  uint8_t prevBottomColor = to_eight_bit(prevBottom);

  if(topColor == prevTopColor && bottomColor == prevBottomColor) {
    str += "▀";
  } else {
    str += "\033[38;5;" + std::to_string(topColor) + 'm' + "\033[48;5;" +
         std::to_string(bottomColor) + "m▀";

         prevTop = top;
         prevBottom = bottom;
  }

  
}
} // namespace eight_bit