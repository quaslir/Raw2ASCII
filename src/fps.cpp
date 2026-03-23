#include "fps.hpp"
#include <iostream>
FPS::FPS() {
  frameCount = 0;
  fps = 0;
  last_time = std::chrono::steady_clock::now();
}

void FPS::update(void) {
  frameCount++;
  auto now = std::chrono::steady_clock::now();
  std::chrono::duration<double> dif = now - last_time;

  if (dif.count() >= 0.5) {
    fps = frameCount / dif.count();
    frameCount = 0;
    last_time = std::chrono::steady_clock::now();
  }
}

std::string FPS::display(void) const {
  std::string buffer;
  buffer.reserve(64);
  buffer += "\033[H\033[1;32m";
  buffer += " FPS: ";
  buffer += std::to_string(fps);
  buffer += " \033[K\033[0m\n";

  return buffer;
}