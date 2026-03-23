#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "utils.hpp"
#include "video.hpp"
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
    std::ifstream file(options.file, std::ios::binary | std::ios::ate);
  try {
  if (options.targetHeight == 1 && options.targetWidth == 1) {
    options.setFullScreen();
  }
  if (isGif(file)) {
    const Gif gif(options.file, options);
    gif.renderGif();
  } else {
    if (options.file.ends_with(".mp4") || options.file.ends_with(".MOV")) {
      VideoDecoder video(options);
      video.renderVideo();
    } else {
      const Image img(options);
      img.renderImage();
    }
  }
} catch(const std::exception & err) {
  std::cerr << err.what() << '\n';
}
}

bool FileManager::isGif(std::ifstream &file) {

  file.seekg(0);
  std::array<char, GIF_SIGNATURE> signature;

  file.read(signature.data(), GIF_SIGNATURE);

  const std::string_view view(signature.data(), 6);
  return view == "GIF87a" || view == "GIF89a";
}
} // namespace ext