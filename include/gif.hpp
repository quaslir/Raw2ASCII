#include "rgb.hpp"
#include "utils.hpp"
#include <memory>
#include <string>
#include <vector>
extern "C" void stbi_image_free(void *);
struct GifFrame {
  std::vector<RGB> frame;
  int delay;
};

class Gif {
private:
  std::vector<GifFrame> data;
  int width, height;
  utils::Options opts;

  std::vector<char> readGif(const std::string &path);

public:
  Gif(const std::string &path, const utils::Options &options);
  void renderGif(void) const;
};