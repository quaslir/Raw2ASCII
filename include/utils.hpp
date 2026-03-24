#pragma once
#include "rgb.hpp"
#include <stdexcept>
#include <string>
#include <vector>
namespace utils {

struct Options {
  int targetWidth = 1;
  int targetHeight = 1;
  bool fullscreen = false;
  bool braille = false;
  bool readStdin = false;
  std::string file = "";
  std::string outputPath = "";
  Options() {}
  Options(int argc, char *argv[]);
  void parse(int argc, char *argv[]);
  Options &operator=(const Options &) = default;
  void setFullScreen(void);
  std::string renderBraille(const std::vector<RGB> &frame) const;
};

std::string calculateBraille(bool dots[2][4]);
std::vector<char> readStdin(void);
std::vector<char> readFile(const std::string &file);
} // namespace utils