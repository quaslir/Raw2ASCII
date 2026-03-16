#define STB_IMAGE_IMPLEMENTATION
#include "image.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
Image::Image(const std::string & pathToImage, const utils::Options & options) : data(nullptr, stbi_image_free) {

unsigned char * raw = stbi_load(pathToImage.c_str(), &width, &height,&channels, 4);

if(!raw) {
    throw std::runtime_error("Failed to load image " + pathToImage);
}

data.reset(reinterpret_cast<RGB*>(raw));

opts = utils::Options(options);

}

void Image::renderImage(void) const {
if(!data) return;

struct winsize window;
ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

int stepX = width / opts.targetWidth;
int stepY = height / opts.targetHeight;

std::stringstream ss;

for(int y = 0; y < height; y += stepY) {
    for(int x = 0; x < width; x += stepX) {
        data[y * width + x].printPixel(ss);
    }
    ss << "\n";
}



if(opts.outputPath.empty())
std::cout << ss.str();
else {
    saveToFile(ss);
}
}

void Image::saveToFile(std::stringstream & ss) const {
std::ofstream file(opts.outputPath);

file << ss.str();
}