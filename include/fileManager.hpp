#pragma once
#include "utils.hpp"
#include <fstream>
#include <string>
namespace ext {

struct FileManager {

  explicit FileManager(utils::Options &&opts);

private:
  utils::Options opts;
  void processFromStdin(void) const;
  void processFromFile(void) const;

  void handleGif(std::string &&data) const;
};
  bool isGif(std::ifstream &file);
  bool isGif(const std::vector<char> &file);
  bool isImg(const std::string &file);
  bool isImg(const std::vector<char> &data);
} // namespace ext