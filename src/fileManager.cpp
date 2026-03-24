#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "utils.hpp"
#include "video.hpp"
#include "stb_image.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <string_view>
#include <array>
#include <exception>
#define GIF_SIGNATURE 6
namespace ext {
FileManager::FileManager(utils::Options && options) {
    opts = options;
    if(opts.readStdin) {
      processFromStdin();
    }

    else {
      processFromFile();
    }
}


void FileManager::processFromFile(void) const {
  std::ifstream file(opts.file, std::ios::binary | std::ios::ate);


if(isGif(file)) {
  std::vector<char>data = utils::readFile(opts.file);
  handleGif(std::move(data));
} else if(isImg(opts.file)) {
  Image img(opts);
  img.renderImage();
}
else {
  VideoDecoder video(opts);
  video.renderVideo();
}
}

void FileManager::processFromStdin(void) const {
  std::vector<char> data = utils::readStdin();

  std::vector<char> windowGif(data.begin(), data.begin() + 6);
  
if(isGif(windowGif)) {
handleGif(std::move(data));
}
else if(isImg(data)){
  Image img(opts, data);
  img.renderImage();
}

else {
  VideoDecoder video(opts);
  video.renderVideo();
}
    
}


bool FileManager::isGif(std::ifstream &file)const {

  file.seekg(0);
  std::array<char, GIF_SIGNATURE> signature;

  file.read(signature.data(), GIF_SIGNATURE);

  const std::string_view view(signature.data(), 6);
  return view == "GIF87a" || view == "GIF89a";
}

bool FileManager::isGif(const std::vector<char> &file)const {

  std::array<char, GIF_SIGNATURE> signature;

  for(int i = 0; i < 6;i++) {
    signature[i] = file[i];
  }

  const std::string_view view(signature.data(), 6);
  return view == "GIF87a" || view == "GIF89a";
}

void FileManager::handleGif(std::vector<char>&&data) const {

Gif gif(std::move(data), opts);
gif.renderGif();
}

bool FileManager::isImg(const std::vector<char> &data) const {
int w, h, ch;

int result = stbi_info_from_memory(reinterpret_cast<const stbi_uc*>(data.data()), data.size(), &w, &h, &ch);

return result == 1;
}

bool FileManager::isImg(const std::string &file) const {
  int w, h, ch;
  return stbi_info(file.c_str(), &w, &h, &ch);
}
} // namespace ext