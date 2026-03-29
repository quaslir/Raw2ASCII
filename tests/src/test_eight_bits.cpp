#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "color8bit.hpp"
#include "rgb.hpp"
#include <vector>
#include <stdint.h>

TEST(eight_bit, to_eight_bit_test) {
std::vector<RGB> pixels = {{0, 0, 0}, {255, 255, 255}, {123, 123, 123}, {131, 207, 44}, {0, 35, 188}};
std::vector<uint8_t> eight_bit = {16, 231, 102, 149, 20};

for(std::size_t i = 0; i < pixels.size(); i++) {
    ASSERT_EQ(eight_bit::to_eight_bit(pixels[i]), eight_bit[i]);
}
}

TEST(eight_bit, is_similar_test) {
std::vector<uint8_t> currValues = {144, 0, 5, 9, 255};
std::vector<uint8_t> prevValues = {142, 5, 3, 6, 0};
std::vector<int> ths = {2, 4, 2, 5, 120};
std::vector<bool> results = {true, false, true, true, false};

for(std::size_t i = 0; i < currValues.size(); i++) {
    ASSERT_THAT(eight_bit::isSimilar(currValues[i], prevValues[i], ths[i]), ::testing::Eq(results[i]));
}
}

TEST(eight_bit, print_pixel_test) {
    std::vector<RGB> topPixels = {
    {0, 0, 0},
    {42, 42, 42},
    {43, 0, 0},
    {43, 0, 0},
    {255, 0, 0}     
};

std::vector<RGB> bottomPixels = {
    {255, 255, 255}, 
    {42, 42, 42},   
    {0, 43, 0},     
    {0, 43, 0},     
    {0, 255, 0}      
};

std::vector<RGB> prevTopPixels = {
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}
};

std::vector<RGB> prevBottomPixels = {
    {255, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 255, 255}
};

std::vector<int> ths = {0, 0, 36, 5, 10};

std::vector<std::string> results = {
    "\033[38;5;16m\033[48;5;231m▀", 
    "▀", 
    "▀", 
    "\033[38;5;52m\033[48;5;22m▀", 
    "\033[38;5;196m\033[48;5;46m▀"
};

for(std::size_t i = 0; i < topPixels.size(); i++) {
        std::string buffer;
    eight_bit::printPixel(buffer, topPixels[i], bottomPixels[i], prevTopPixels[i], prevBottomPixels[i], ths[i]);
    ASSERT_THAT(buffer, ::testing::HasSubstr(results[i])) << "Failed at index " << i;;
}
}