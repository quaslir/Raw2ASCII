#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
Image::Image(const std::string & pathToImage) : data(nullptr, stbi_image_free) {

unsigned char * raw = stbi_load(pathToImage.c_str(), &width, &height,&channels, 3);

if(!raw) {
    throw std::runtime_error("Failed to load image " + pathToImage);
}

data.reset(reinterpret_cast<RGB*>(raw));

}

void Image::renderImage(void) const {

std::string ss;


for(int i = 0; i < width * height; i++) {
    data[i].printPixel(ss);
}
}