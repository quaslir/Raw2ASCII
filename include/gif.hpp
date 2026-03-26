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


public:
  Gif(std::vector<char> &&buffer, const utils::Options &options);
  void renderGif(void) const;
};