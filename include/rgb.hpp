#include <stdint.h>
#include <sstream>
#pragma once
#pragma pack(push, 1)
struct RGB {
    uint8_t r, g, b, alpha;

    void printPixel(std::stringstream & ss, const RGB & bottom, RGB & prevTop, RGB & prevBottom) const;
    bool operator==(const RGB& other) const;
    bool operator!=(const RGB& other) const;
};

#pragma pack(pop)