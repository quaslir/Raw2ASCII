#include "image.hpp"
#include "utils.hpp"
#include "gif.hpp"
#include <string>
#include <exception>
#include <iostream>
int main(int argc, char * argv[]) {

    if(argc < 2) {
        std::cerr << "Path to media was not provided" << std::endl;
        return 1;
    }

    std::string path = argv[1];

    utils::Options opts (argc, argv);


    try {
        if(path.ends_with(".gif")) {
            Gif gif (path);
        } else {
    Image img(path, opts);
    img.renderImage();
        }
    } catch(const std::exception & err) {
        std::cerr << err.what() << std::endl;
    }
    return 0;
}