#include "rgb.hpp"
#include <memory>
#include "utils.hpp"
#include <string>
extern "C" void stbi_image_free(void*);
struct GifFrame{
std::vector<RGB> frame;
int delay;

};

class Gif {
    private:
    std::vector<GifFrame> data;
    int width, height;
    utils::Options opts;

    std::vector<unsigned char> readGif(const std::string &path);
    public:
    Gif(const std::string & path);
};