#include "utils.hpp"
#include <iostream>
#include <exception>
namespace utils{

    Options::Options() {
        targetWidth = 1;
        targetHeight = 1;
        fullscreen = false;
        outputPath = "";
    }

    Options::Options(const Options & opts) {
        targetWidth = opts.targetWidth;
        targetHeight = opts.targetHeight;
        fullscreen = opts.fullscreen;
        outputPath = opts.outputPath;
    }

    Options Options::parse(int argc, char * argv[]) {
        if(argc < 3) return *this;
        for(int i = 2; i < argc; i++) {
             std::string_view param(argv[i]);
            bool longForm = param[1] == '-';

             if((param.substr(0,2) == "-w") || (param.substr(0,8) == "--width=")) {
                size_t index = longForm ? 8 : 2;
                try {
                    int w = std::stoi(std::string{param.substr(index)});
                    targetWidth = w;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetWidth = 1;

                }
             }

             else if((param.substr(0,2) == "-h") || (param.substr(0, 9) == "--height=")) {
                size_t index = longForm ? 9 : 2;
                try {
                    int h = std::stoi(std::string{param.substr(index)});
                    targetHeight = h;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetHeight = 1;

                }
             }

             else if(param.substr(0,5) == "--fit") {
                fullscreen = 1;
             }

             else if((param.substr(0,2) == "-o") || (param.substr(0, 9) == "--output=")) {
                size_t index = longForm ? 10 : 2;
                outputPath = param.substr(index);
             }
        }
       return *this;
    }
}