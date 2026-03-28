#include "rgb.hpp"
#include "utils.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using namespace utils;
TEST(test_utils, simple_parsing) {
  int argc = 2;
  char *argv[] = {(char *)"./RAW2ASCII", (char *)"-"};
  Options opts;
  opts.parse(argc, argv);

  EXPECT_THAT(opts,
              ::testing::AllOf(
                  ::testing::Field(&Options::braille, ::testing::IsFalse()),
                  ::testing::Field(&Options::file, ::testing::IsEmpty()),
                  ::testing::Field(&Options::fps, ::testing::IsFalse()),
                  ::testing::Field(&Options::help, ::testing::IsFalse()),
                  ::testing::Field(&Options::readStdin, ::testing::IsTrue())));
}

TEST(test_utils, width_height_parsing) {
  int argc = 4;
  char *argv[] = {(char *)"./RAW2ASCII", (char *)"-", (char *)"-w200",
                  (char *)"-h70"};
  Options opts;
  opts.parse(argc, argv);

  EXPECT_THAT(opts,
              ::testing::AllOf(
                  ::testing::Field(&Options::braille, ::testing::IsFalse()),
                  ::testing::Field(&Options::file, ::testing::IsEmpty()),
                  ::testing::Field(&Options::fps, ::testing::IsFalse()),
                  ::testing::Field(&Options::help, ::testing::IsFalse()),
                  ::testing::Field(&Options::readStdin, ::testing::IsTrue()),
                  ::testing::Field(&Options::targetHeight, ::testing::Eq(70)),
                  ::testing::Field(&Options::targetWidth, ::testing::Eq(200))));
}

TEST(test_utils, exceptions) {
  {
    int argc = 4;
    char *argv[] = {(char *)"./RAW2ASCII", (char *)"-", (char *)"-w200width",
                    (char *)"-h70"};
    Options opts;
    EXPECT_THROW(opts.parse(argc, argv), std::invalid_argument);
  }
  {
    int argc = 4;
    char *argv[] = {(char *)"./RAW2ASCII", (char *)"-", (char *)"-w2000",
                    (char *)"-h1000"};
    Options opts;
    EXPECT_THROW(opts.parse(argc, argv), std::invalid_argument);
  }
  {
    int argc = 4;
    char *argv[] = {(char *)"./RAW2ASCII", (char *)"-", (char *)"-w200",
                    (char *)"-h70"};
    Options opts;
    EXPECT_NO_THROW(opts.parse(argc, argv));
  }
}

TEST(test_utils, long_forms) {
  {
    int argc = 6;
    char *argv[] = {(char *)"./RAW2ASCII",   (char *)"example.mp4",
                    (char *)"--width=200",   (char *)"--height=70",
                    (char *)"--threshold=5", (char *)"--fps"};

    Options opts;
    opts.parse(argc, argv);
    EXPECT_THAT(
        opts,
        ::testing::AllOf(
            ::testing::Field(&Options::braille, ::testing::IsFalse()),
            ::testing::Field(&Options::file, ::testing::StrEq("example.mp4")),
            ::testing::Field(&Options::fps, ::testing::IsTrue()),
            ::testing::Field(&Options::help, ::testing::IsFalse()),
            ::testing::Field(&Options::readStdin, ::testing::IsFalse()),
            ::testing::Field(&Options::targetHeight, ::testing::Eq(70)),
            ::testing::Field(&Options::targetWidth, ::testing::Eq(200))));
  }
}

TEST(test_utils, isSimilarTest) {
  RGB pixel1(95, 63, 35);
  RGB pixel2(90, 60, 30);

  EXPECT_FALSE(isSimilar(pixel1, pixel2, 7));
  EXPECT_TRUE(isSimilar(pixel1, pixel2, 8));
}