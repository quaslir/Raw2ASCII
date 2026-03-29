#include "rgb.hpp"
#include "utils.hpp"
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
                     RGB &prevBottom, int th) const {
  bool topTransparent = alpha < 10;
  bool bottomTransparent = bottom.alpha < 10;
  bool transparencyChanged = ((topTransparent != (prevTop.alpha < 10)) ||
                              (bottomTransparent != (bottom.alpha < 10)));
  if (!transparencyChanged && utils::isSimilar(*this, prevTop, th) &&
      utils::isSimilar(bottom, prevBottom, th)) {
    str += ((topTransparent && bottomTransparent) ? " " : "▀");
    return;
  } 
   int Rt = (r * alpha + 128) / 255;
  int Gt = (g * alpha + 128) / 255;
  int Bt = (b * alpha + 128) / 255;
  int Rb = (bottom.r * bottom.alpha + 128) / 255;
  int Gb = (bottom.g * bottom.alpha + 128) / 255;
  int Bb = (bottom.b * bottom.alpha + 128) / 255;
  
  if(!transparencyChanged && utils::isSimilar(*this, prevTop, th)) {
    str += "\033[48;2;" + std::to_string(Rb) + ';' +
           std::to_string(Gb) + ';' + std::to_string(Bb) + "m▀";
           prevBottom = RGB(Rb, Gb, Bb);
           return;
  }

  else if(!transparencyChanged && utils::isSimilar(bottom, prevBottom, th)) {
     str += "\033[38;2;" + std::to_string(Rt) + ';' +
           std::to_string(Gt) + ';' + std::to_string(Bb) + "m▀";
           prevTop = RGB(Rt, Gt, Bt);
           return;
  }

  if (topTransparent || bottomTransparent) {
    str += "\033[0m ";
    prevTop = RGB();
    prevBottom = RGB();
  } else {

    
    str += "\033[38;2;" + std::to_string(Rt) + ';' + std::to_string(Gt) + ';' +
           std::to_string(Bt) + 'm' + "\033[48;2;" + std::to_string(Rb) + ';' +
           std::to_string(Gb) + ';' + std::to_string(Bb) + "m▀";
    prevTop = RGB(Rt, Gt, Bt);
    prevBottom = RGB(Rb, Gb, Bb);
  }
}
