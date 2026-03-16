#pragma once
#include <string>
#include <memory>
#include "rgb.hpp"
#include "utils.hpp"
extern "C" void stbi_image_free(void*);
class Image {
    using StbPtr = std::unique_ptr<RGB[], decltype(&stbi_image_free)>;
    private:
    int width, height, channels;
    StbPtr data;
    utils::Options opts;
    public:
    Image(const std::string & pathToImage, const utils::Options & options);
    void renderImage(void) const;
};