#include "image.hpp"
#include <string>

int main(int argc, char * argv[]) {
    if(argc < 2) return 1;

    std::string path = argv[1];

    Image img(path);
    img.renderImage();
    
    return 0;
}