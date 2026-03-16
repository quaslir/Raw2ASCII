#include "rgb.hpp"
#include <cstdio>
#include <iostream>
void RGB::printPixel(std::string & ss) const {
char buffer[64];
std::snprintf(buffer, sizeof(buffer), "\x1b[38;2;%u;%u;%um@\x1b[0m", r, g, b);

std::cout << buffer << std::endl;
}