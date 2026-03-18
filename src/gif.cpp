#include "gif.hpp"
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
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

Gif::Gif(const std::string &path){
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
};
