#include "rgb.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

TEST(test_rgb, easy_print_pixel) {
  std::string buffer;
  RGB rgb, prevTop, prevBottom;
  rgb.printPixel(buffer, RGB(), prevTop, prevBottom);

  ASSERT_EQ(buffer, " ");
}

TEST(test_rgb, medium_print_pixel) {
  std::string buffer;
  RGB top(144, 200, 200, 255);
  RGB bottom{top};
  RGB prevTop(134, 190, 195, 255);

  top.printPixel(buffer, bottom, prevTop, prevTop, 16);
  EXPECT_EQ(buffer, "▀");
  buffer.clear();
  prevTop = RGB(134, 190, 195, 255);
  top.printPixel(buffer, bottom, prevTop, prevTop, 15);
  ASSERT_NE(buffer, "▀");
  EXPECT_EQ(buffer, "\033[38;2;144;200;200m\033[48;2;144;200;200m▀");
}

TEST(test_rgb, alpha_zero) {
  std::string buffer;
  RGB top(255, 0, 0, 0);
  RGB bottom(0, 255, 0, 255);
  RGB prev(0, 0, 0, 255);

  top.printPixel(buffer, bottom, prev, prev);

  EXPECT_THAT(buffer, ::testing::Not(::testing::HasSubstr("255;0;0")));
}

TEST(test_rgb, operator_eq) {
  RGB pixel1(123, 255, 144, 255);
  RGB pixel2(123, 255, 144, 0);
  EXPECT_TRUE(pixel1 == pixel2);
  pixel1.r++;
  EXPECT_FALSE(pixel1 == pixel2);
}