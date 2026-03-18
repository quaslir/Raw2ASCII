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

Options::Options(int argc, char * argv[]) {
    parse(argc, argv);
}
    void Options::parse(int argc, char * argv[]) {
        if(argc < 3) return;
        for(int i = 2; i < argc; i++) {
             std::string_view param(argv[i]);


             if((param.starts_with("-w")) || (param.starts_with("--width="))) {
                size_t index = param.starts_with("-w") ? std::string{"-w"}.length() : std::string{"--width="}.length();
                try {
                    int w = std::stoi(std::string{param.substr(index)});
                    targetWidth = w;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetWidth = 1;

                }
             }

             else if((param.starts_with("-h")) || (param.starts_with("--height"))) {
                size_t index = param.starts_with("-h") ? std::string{"-h"}.length() : std::string{"--height="}.length();
                try {
                    int h = std::stoi(std::string{param.substr(index)});
                    targetHeight = h;
                } catch(const std::exception & err) {
                    std::cerr << err.what() << std::endl;
                    targetHeight = 1;

                }
             }

             else if(param.starts_with("--fit")) {
                fullscreen = 1;
             }

             else if((param.starts_with("-o")) || (param.starts_with("--output="))) {
                size_t index = param.starts_with("-o") ? std::string{"-o"}.length() : std::string{"--output="}.length();
                outputPath = param.substr(index);
             }
        }
    }
}