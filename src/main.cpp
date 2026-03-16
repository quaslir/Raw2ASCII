#include "image.hpp"
#include "utils.hpp"
#include <string>
#include <exception>
#include <iostream>
int main(int argc, char * argv[]) {

    if(argc < 2) {
        std::cerr << "Path to media was not provided" << std::endl;
        return 1;
    }

    std::string path = argv[1];

    utils::Options opts;

    opts.parse(argc, argv);

    try {
    Image img(path, opts);
    img.renderImage();
    } catch(const std::exception & err) {
        std::cerr << err.what() << std::endl;
    }
    return 0;
}