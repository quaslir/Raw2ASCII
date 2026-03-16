#pragma once
#include <string>
#include <memory>

using SdbPtr = std::unique_ptr<unsigned char[], void(*)(void*)>;

class Image {
    private:
    int width, height, channels;
    SdbPtr data;

    public:
    Image(const std::string & pathToImage);
    ~Image();
};