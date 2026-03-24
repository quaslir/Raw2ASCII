#pragma once
#include <fstream>
#include <string>
#include "utils.hpp"
namespace ext {

struct FileManager {

  FileManager(utils::Options && opts);


  private:

  utils::Options opts;
    void processFromStdin(void) const;
  void processFromFile(void) const;
  bool isGif(std::ifstream &file) const;
  bool isGif(const std::vector<char> &file) const;
  bool isImg(const std::string &file) const;
  bool isImg(const std::vector<char>&data) const;
  void handleGif(std::vector<char>&&data) const;
};

} // namespace ext