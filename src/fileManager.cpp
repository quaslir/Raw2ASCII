#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "utils.hpp"
#include "video.hpp"
#include <iostream>
#include <stdexcept>
#include <string_view>
#define GIF_SIGNATURE 6
namespace ext {
FileManager::FileManager(const std::string &path, int argc, char *argv[]) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("could not open " + path);
  }
  utils::Options options(argc, argv);
  if (options.targetHeight == 1 && options.targetWidth == 1) {
    options.setFullScreen();
  }
  if (isGif(file)) {
    Gif gif(path, options);
    gif.renderGif();
  } else {
    if (path.ends_with(".mp4")) {
      VideoDecoder video(path, options);
      video.renderVideo();
    } else {
      const Image img(path, options);
      img.renderImage();
    }
  }
}

bool FileManager::isGif(std::ifstream &file) {

  file.seekg(0);

  char signature[GIF_SIGNATURE];

  file.read(signature, GIF_SIGNATURE);

  const std::string_view view(signature, 6);
  return view == "GIF87a" || view == "GIF89a";
}
} // namespace ext