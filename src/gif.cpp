#include "gif.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
std::vector<unsigned char> Gif::readGif(const std::string &path) {
std::ifstream file(path, std::ios::binary | std::ios::ate);

if(!file.is_open()) {
    throw std::runtime_error("file " + path + " could not be found");
}

std::streamsize size = file.tellg();
file.seekg(0, std::ios::beg);

std::vector<unsigned char> buffer;
buffer.resize(size);

if(!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    throw std::runtime_error("could not read file " + path);
}
return buffer;

}

Gif::Gif(const std::string &path, const utils::Options & options){
    int w, h, count, channels;
    int * delays = nullptr;
    
    std::vector<unsigned char> buffer = readGif(path);
    unsigned char * raw = stbi_load_gif_from_memory(buffer.data(), static_cast<int>(buffer.size())
    , &delays, &w, &h, &count, &channels, 4);   

    if(!raw) {
        throw std::runtime_error("Failed to load GIF: " + path);
    }

    width = w;
    height = h;

size_t frameSize = width * height * 4;

data.resize(count);

for(int i = 0; i < count; i++) {
    unsigned char * framePtr = raw + (i * frameSize);

    data[i].delay = delays[i];
    data[i].frame.resize(width * height);
    std::memcpy(data[i].frame.data(), framePtr, frameSize);


}
    stbi_image_free(raw);

    stbi_image_free(delays);

    opts = options;
};


void Gif::renderGif(void) const {

int stepX = width / opts.targetWidth;
int stepY = height / opts.targetHeight;

std::cout << "\033[2J\033[?25l";

for(size_t i = 0; i < data.size(); i++) {
    std::cout << "\033[H";
    std::stringstream ss;

    for(int y = 0; y < height; y += stepY * 2) {
    RGB prevTop = {0, 0, 0, 0};
    RGB prevBottom = {0, 0, 0, 0};
    for(int x = 0; x < width; x += stepX) {

        const RGB& top = data[i].frame[y * width + x];
        int bottomIdx = (y + stepY < height) ? (y + stepY) : y;
        const RGB& bottom = data[i].frame[(bottomIdx) * width + x];

       top.printPixel(ss, bottom, prevTop, prevBottom);
    }
    ss << "\x1b[0m\n";
}

std::cout << ss.str();
std::this_thread::sleep_for(std::chrono::milliseconds(data[i].delay));
}

std::cout << "\033[?25h" << std::endl;
}