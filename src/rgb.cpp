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

  int Rt = (r * alpha + 128) / 255;
  int Gt = (g * alpha + 128) / 255;
  int Bt = (b * alpha + 128) / 255;
  int Rb = (bottom.r * bottom.alpha + 128) / 255;
  int Gb = (bottom.g * bottom.alpha + 128) / 255;
  int Bb = (bottom.b * bottom.alpha + 128) / 255;

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
    char buffer[100];
    std::snprintf(buffer, sizeof(buffer),
                  "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀", Rt, Gt, Bt, Rb, Gb,
                  Bb);

    str += buffer;
  }

  prevTop = *this;
  prevBottom = bottom;
}

