#include <stdint.h>
#include <sstream>
#pragma once
#pragma pack(push, 1)
struct RGB {
    uint8_t r, g, b, alpha;

    void printPixel(std::stringstream & ss, const RGB & bottom, RGB & prevTop, RGB & prevBottom) const;
    bool operator==(const RGB& other) const;
    bool operator!=(const RGB& other) const;

    RGB() : r(0), g(0), b(0), alpha(255) {}
    RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue), alpha(255) {}
};

#pragma pack(pop)