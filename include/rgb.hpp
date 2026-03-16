#pragma pack(push, 1)
#include <stdint.h>
#include <sstream>

struct RGB {
    uint8_t r, g, b;

    void printPixel(std::stringstream & ss) const;
};

#pragma pack(pop)