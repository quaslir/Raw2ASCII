#include "utils.hpp"
#include <iostream>
#include <exception>
namespace utils{

    Options::Options() {
        targetWidth = 0;
        targetHeight = 0;
    }

    Options::Options(const Options & opts) {
        targetWidth = opts.targetWidth;
        targetHeight = opts.targetHeight;
    }

    Options Options::parse(int argc, char * argv[]) {
        if(argc < 3) return *this;
        for(int i = 2; i < argc; i++) {
             std::string_view param(argv[i]);

             if(param.substr(0,2) == "-w") {
                try {
                    int w = std::stoi(std::string{param.substr(2)});
                    targetWidth = w;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetWidth = 1;

                }
             }

             else if(param.substr(0,2) == "-h") {
                try {
                    int h = std::stoi(std::string{param.substr(2)});
                    targetHeight = h;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetHeight = 1;

                }
             }
        }
       return *this;
    }
}