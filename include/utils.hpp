#pragma once
#include <string>
#include <stdexcept>

namespace utils {

struct Options {
    int targetWidth;
    int targetHeight;
    bool fullscreen;
    std::string outputPath;
    Options();
    Options(int argc, char * argv[]);
    void parse(int argc, char * argv[]);
    Options& operator=(const Options &) = default;
};

}