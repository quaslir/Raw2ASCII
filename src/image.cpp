#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
Image::Image(const std::string & pathToImage) : data(nullptr, stbi_image_free) {

unsigned char * raw = stbi_load(pathToImage.c_str(), &width, &height,&channels, 3);

if(!raw) {
    throw std::runtime_error("Failed to load image " + pathToImage);
}

data.reset(reinterpret_cast<RGB*>(raw));

}

void Image::renderImage(void) const {
if(!data) return;

struct winsize window;
ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

int stepX = 17;
int stepY = 34;

std::stringstream ss;



for(int y = 0; y < height; y += stepY) {
    for(int x = 0; x < width; x += stepX) {
        data[y * width + x].printPixel(ss);
    }
    ss << "\n";
}
std::cout << ss.str();
}