#pragma once
#include <fstream>
#include <string>
#include "utils.hpp"
namespace ext {

struct FileManager {

  FileManager(utils::Options && opts);

  bool isGif(std::ifstream &file);
};

} // namespace ext