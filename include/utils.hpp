#pragma once
#include <string>
#include <stdexcept>

namespace utils {

struct Options {
    int targetWidth;
    int targetHeight;

    Options();
    Options(const Options &opts);
    Options parse(int argc, char * argv[]);
    Options& operator=(const Options &) = default;
};

}