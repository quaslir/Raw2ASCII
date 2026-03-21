#include "rgb.hpp"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
bool RGB::operator==(const RGB &other) const {
  if (this->r == other.r && this->g == other.g && this->b == other.b)
    return true;
  return false;
}

bool RGB::operator!=(const RGB &other) const { return !(*this == other); }

void RGB::printPixel(std::string &str, const RGB &bottom, RGB &prevTop,
                     RGB &prevBottom) const {

  int Rt = (r * alpha) / 255;
  int Gt = (g * alpha) / 255;
  int Bt = (b * alpha) / 255;
  int Rb = (bottom.r * bottom.alpha) / 255;
  int Gb = (bottom.g * bottom.alpha) / 255;
  int Bb = (bottom.b * bottom.alpha) / 255;

  bool isTransparent = (alpha < 10 && bottom.alpha < 10);

  if (prevTop.r == Rt && prevTop.g == Gt && prevTop.b == Bt &&
      prevBottom.r == Rb && prevBottom.g == Gb && prevBottom.b == Bb &&
      prevTop.alpha == (isTransparent ? 0 : 255)) {
    str += (isTransparent ? " " : "▀");
    return;
  }

  if (isTransparent) {
    str += "\033[0m ";
    prevTop = RGB();
    prevBottom = RGB();
  } else {
    char buffer[128];
    std::snprintf(buffer, sizeof(buffer),
                  "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀", Rt, Gt, Bt, Rb, Gb,
                  Bb);

    str += buffer;
  }

  prevTop = *this;
  prevBottom = bottom;
}

RGB getContrastColor(const RGB & pixel) {
  const int minContrast = 128;
  int maxContrast = 192;
  int y = static_cast<int>(std::round(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b));
    
  bool bright = (y < 128);

  if(bright) {
    return RGB(
      std::clamp(pixel.r + minContrast, 0, 255),
      std::clamp(pixel.g + minContrast, 0, 255),
      std::clamp(pixel.b + minContrast, 0, 255)
    );
  } else {
    return RGB(pixel.r / 4, pixel.g / 4, pixel.b / 4);
  }
}