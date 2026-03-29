#include "rgb.hpp"
#include <stdint.h>
#include <string>
#pragma once
namespace eight_bit {

uint8_t to_eight_bit(const RGB &pixel);
void printPixel(std::string &str, const RGB &top, const RGB &bottom,
                RGB &prevTop, RGB &prevBottom, int th = 1);
bool isSimilar(uint8_t cur, uint8_t prev, int th);

} // namespace eight_bit