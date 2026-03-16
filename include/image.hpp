#pragma once
#include <string>
#include <memory>
#include "rgb.hpp"

extern "C" void stbi_image_free(void*);
class Image {
    using StbPtr = std::unique_ptr<RGB[], decltype(&stbi_image_free)>;
    private:
    int width, height, channels;
    StbPtr data;

    public:
    Image(const std::string & pathToImage);
    void renderImage(void) const;
};