#pragma once
#include "utils.hpp"
#include <fstream>
#include <string>
namespace ext {

struct FileManager {

  explicit FileManager(utils::Options &&options);
  void open(void);

private:
  utils::Options opts;
  void processFromStdin(void);
  void processFromFile(void);

  void handleGif(std::string &&data);
};
bool isGif(std::ifstream &file);
bool isGif(const std::vector<char> &file);
bool isImg(const std::string &file);
bool isImg(const std::string &data, bool flag);
} // namespace ext