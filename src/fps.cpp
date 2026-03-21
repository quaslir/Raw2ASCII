#include "fps.hpp"

FPS::FPS() {
    frameCount = 0;
    fps = 0;
    last_time = std::chrono::steady_clock::now();
}

void FPS::update(void) {
    frameCount++;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> dif = now - last_time;

    if(dif.count() >= 0.5) {
        fps = frameCount / dif.count();
        frameCount = 0;
        last_time = std::chrono::steady_clock::now();
    }
}

int FPS::getfps(void) const {
    return fps;
}