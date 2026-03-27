#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "stb_image.h"
#include "utils.hpp"
#include "video.hpp"
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#define GIF_SIGNATURE 6
namespace ext {
FileManager::FileManager(utils::Options &&options) {
  opts = options;
  if (opts.readStdin) {
    processFromStdin();
  }

  else {
    processFromFile();
  }
}

void FileManager::processFromFile(void) const {
  std::ifstream file(opts.file, std::ios::binary | std::ios::ate);
    try {
  if (isGif(file)) {
    std::string data = utils::readFile(opts.file);
    handleGif(std::move(data));
  } else if (isImg(opts.file)) {
    Image img(opts);
    img.renderImage();
  } else {

    VideoDecoder video{opts};
    video.open();
    video.renderVideo();
    
  }
  } catch(const std::exception &err) {
      std::cerr << err.what() << std::endl;
    }
}

void FileManager::processFromStdin(void) const {
  std::string header;
  constexpr std::size_t chunk = 1024 * 1024;
  header.resize(chunk);
  size_t bytesRead = std::fread(header.data(), 1, chunk, stdin);
  if(bytesRead == 0) return;
  header.resize(bytesRead);
  std::vector<char> windowGif(header.data(), header.data() + 6);
  try {
  if (isGif(windowGif)) {
    std::string data = utils::readStdin();
   header.append(data);
    handleGif(std::move(header));
  } else if (isImg(header, 1)) {
   std::string data = utils::readStdin();
   header.append(data);
    Image img(opts, std::move(header));
    img.renderImage();
  }

  else {
    VideoDecoder video{opts};
    video.setHeader(std::move(header));
    video.open();
    video.renderVideo();
  }
} catch(const std::exception &err) {
  std::cerr << err.what() << std::endl;
}
}

bool isGif(std::ifstream &file) {
  file.seekg(0);
  std::array<char, GIF_SIGNATURE> signature;

  file.read(signature.data(), GIF_SIGNATURE);

  const std::string_view view(signature.data(), 6);
  return view == "GIF87a" || view == "GIF89a";
}

bool isGif(const std::vector<char> &file) {
  if(file.empty()) return false;
  std::array<char, GIF_SIGNATURE> signature;

  for (int i = 0; i < 6; i++) {
    signature[i] = file[i];
  }

  const std::string_view view(signature.data(), 6);
  return view == "GIF87a" || view == "GIF89a";
}

void FileManager::handleGif(std::string &&data) const {
  Gif gif(std::move(data), opts);
  gif.renderGif();
}

bool isImg(const std::string &data, bool flag) {
  if(data.empty()) return false;
  int w, h, ch;

  int result = stbi_info_from_memory(
      reinterpret_cast<const stbi_uc *>(data.data()), data.size(), &w, &h, &ch);
  return result == 1;
}

bool isImg(const std::string &file) {
  if(file.empty()) return false;
  int w, h, ch;
  return stbi_info(file.c_str(), &w, &h, &ch);
}
} // namespace ext