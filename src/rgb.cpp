#include "rgb.hpp"
#include <cstdio>
#include <iostream>

bool RGB::operator==(const RGB& other) const {
if(this->r == other.r && this->g == other.g && this->b == other.b) return true;
return false;
}

bool RGB::operator!=(const RGB& other) const {
return !(*this == other);
}


void RGB::printPixel(std::stringstream & ss, const RGB & bottom, RGB & prevTop, RGB & prevBottom) const {

    if(this->alpha == 0 && bottom.alpha == 0) {
        ss << " ";
        return;
    }

        int Rt = (this->alpha > 128) ? r : 0;
        int Gt = (this->alpha > 128) ? g : 0;
        int Bt = (this->alpha > 128) ? b : 0;
        int Rb = (this->alpha > 128) ? bottom.r : 0;
        int Gb = (this->alpha > 128) ? bottom.g : 0;
        int Bb = (this->alpha > 128) ? bottom.b : 0;

        char buffer[128];

        std::snprintf(buffer, sizeof (buffer), "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀\033[0m",
    Rt, Gt, Bt, Rb, Gb, Bb);

    ss << buffer;

}