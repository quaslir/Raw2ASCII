#include "image.hpp"
#include "utils.hpp"
#include "fileManager.hpp"
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

    ext::FileManager manager(path, argc, argv);
    return 0;
}