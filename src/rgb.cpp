#include "rgb.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
bool RGB::operator==(const RGB &other) const {
  if (this->r == other.r && this->g == other.g && this->b == other.b)
    return true;
  return false;
}

bool RGB::operator!=(const RGB &other) const { return !(*this == other); }


void RGB::printPixel(std::string &str, const RGB &bottom, RGB &prevTop,
                     RGB &prevBottom) const {

  int Rt = (r * alpha + 128) / 255;
  int Gt = (g * alpha + 128) / 255;
  int Bt = (b * alpha + 128) / 255;
  int Rb = (bottom.r * bottom.alpha + 128) / 255;
  int Gb = (bottom.g * bottom.alpha + 128) / 255;
  int Bb = (bottom.b * bottom.alpha + 128) / 255;
  bool isTransparent = (alpha < 10 && bottom.alpha < 10);
  bool prevTransparent = (prevTop.alpha < 10 && prevBottom.alpha < 10);
  if (prevTop.r == Rt && prevTop.g == Gt && prevTop.b == Bt &&
      prevBottom.r == Rb && prevBottom.g == Gb && prevBottom.b == Bb && isTransparent == prevTransparent) {
    str += (isTransparent ? " ": "▀");
    return;
  }

  if (isTransparent) {
    str += "\033[0m ";
    prevTop = RGB();
    prevBottom = RGB();
  } else {

    str += "\033[38;2;" + std::to_string(Rt) + ';' + std::to_string(Gt) + ';' +
           std::to_string(Bt) + 'm' + "\033[48;2;" + std::to_string(Rb) + ';' +
           std::to_string(Gb) + ';' + std::to_string(Bb) + "m▀";
  }

  prevTop = *this;
  prevBottom = bottom;
}
