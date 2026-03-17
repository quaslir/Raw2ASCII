#define STB_IMAGE_IMPLEMENTATION
#include "gif.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

std::vector<unsigned char> Gif::readGif(const std::string &path) {
std::ifstream file(path, std::ios::binary | std::ios::ate);

if(!file.is_open()) {
    throw std::runtime_error("file " + path + " could not be found");
}

std::streamsize size = file.tellg();
file.seekg(0, std::ios::beg);

std::vector<unsigned char> buffer;
buffer.reserve(size);

if(!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    throw std::runtime_error("could not read file " + path);
}
return buffer;

}

Gif::Gif(const std::string &path) {
    int w, h, count, channels;
    int * delays = nullptr;


};