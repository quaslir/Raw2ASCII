#include "rgb.hpp"
#include <cstdio>
#include <iostream>
void RGB::printPixel(std::stringstream & ss) const {
if(alpha == 0) {
    ss << " ";
    return;
}

const static std::string ramp = " .:-=+*#%@";
char symbol = ramp[alpha * (ramp.size() - 1) / 255];
char buffer[64];
std::snprintf(buffer, sizeof(buffer), "\x1b[38;2;%u;%u;%um%c\x1b[0m", r, g, b, symbol);

ss << buffer;
}