#pragma once
#include <string>
#include <stdexcept>

namespace utils {

struct Options {
    int targetWidth = 1;
    int targetHeight = 1;
    bool fullscreen = false;
    std::string outputPath = "";
    Options() {}
    Options(int argc, char * argv[]);
    void parse(int argc, char * argv[]);
    Options& operator=(const Options &) = default;
    void setFullScreen(void);
};

}