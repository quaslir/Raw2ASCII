#include "utils.hpp"
#include <exception>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
namespace utils {

Options::Options(int argc, char *argv[]) { parse(argc, argv); }
void Options::parse(int argc, char *argv[]) {

  if (argc < 3)
    return;
  for (int i = 2; i < argc; i++) {
    std::string_view param(argv[i]);

    if ((param.starts_with("-w")) || (param.starts_with("--width="))) {
      size_t index = param.starts_with("-w") ? std::string{"-w"}.length()
                                             : std::string{"--width="}.length();
      try {
        int w = std::stoi(std::string{param.substr(index)});
        targetWidth = w;
      } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        targetWidth = 1;
      }
    }

    else if ((param.starts_with("-h")) || (param.starts_with("--height"))) {
      size_t index = param.starts_with("-h")
                         ? std::string{"-h"}.length()
                         : std::string{"--height="}.length();
      try {
        int h = std::stoi(std::string{param.substr(index)});
        targetHeight = h;
      } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        targetHeight = 1;
      }
    }

    else if (param.starts_with("--fit")) {
      setFullScreen();
    }

    else if ((param.starts_with("-o")) || (param.starts_with("--output="))) {
      size_t index = param.starts_with("-o")
                         ? std::string{"-o"}.length()
                         : std::string{"--output="}.length();
      outputPath = param.substr(index);
    }

    else if (param == "--braille" || param == "-b") {
      braille = true;
    }
  }
}

void Options::setFullScreen(void) {
  struct winsize window;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
  targetWidth = window.ws_col * 0.8;
  targetHeight = window.ws_row * 0.8;
}
std::string calculateBraille(bool dots[2][4]) {
  int byte = 0;
  if (dots[0][0])
    byte |= 0x01;
  if (dots[0][1])
    byte |= 0x02;
  if (dots[0][2])
    byte |= 0x04;
  if (dots[1][0])
    byte |= 0x08;
  if (dots[1][1])
    byte |= 0x10;
  if (dots[1][2])
    byte |= 0x20;
  if (dots[0][3])
    byte |= 0x40;
  if (dots[1][3])
    byte |= 0x80;

  std::string result;

  result += (char)(0xE2);
  result += (char)(0xA0 + (byte >> 6));
  result += (char)(0x80 + (byte & 0x3F));

  return result;
}

void Options::renderBraille(const std::vector<RGB> & frame) const {
  std::string buffer;
  buffer += "\033[H";

  for (int y = 0; y < targetHeight; y += 4) {
    RGB prevbright, prevdark;
    
    for (int x = 0; x < targetWidth; x += 2) {

      long sumRed = 0, sumGreen = 0, sumBlue = 0;
      RGB dark, bright;
      int minLum = 256;
      int maxLum = -1;
      for (int dy = 0; dy < 4; dy++) {
        for (int dx = 0; dx < 2; dx++) {

          int currentX = dx + x;
          int currentY = dy + y;

          if (currentX < targetWidth && currentY < targetHeight) {

            const RGB &p = frame[(y + dy) * targetWidth + (x + dx)];
            sumRed += p.r;
            sumGreen += p.g;
            sumBlue += p.b;

            int lum = (p.r * 213 + p.g * 715 + p.b * 72) / 1000;
            if(lum < minLum) {
              minLum = lum;
              dark = p;
            }
             if(lum > maxLum) {
              maxLum = lum;
              bright = p;
             }
          }
        }
      }
      
      int contrast = maxLum - minLum;
      int threshold = (minLum + maxLum) / 2;
      bool dots[2][4] = {0};
      if(contrast > 25) {
      for(int dy = 0; dy < 4; dy++) {
        for(int dx = 0; dx < 2; dx++) {

           int currentX = dx + x;
          int currentY = dy + y;

          if (currentX < targetWidth && currentY < targetHeight) {
            const RGB &p = frame[currentY * targetWidth + currentX];
            int pLum = (p.r * 213 + p.g * 715 + p.b * 72) / 1000;

            dots[dx][dy] = (pLum > threshold);
          }
        }
      }

    }
    else {
      bright = dark;
    }
      if(bright != prevbright) {
        buffer += "\033[38;2;" + std::to_string(bright.r) + ';' +
                  std::to_string(bright.g) + ';' +
                  std::to_string(bright.b) + 'm';
      }

      if(dark != prevdark) {
        buffer += "\033[48;2;" + std::to_string(dark.r) + ';' +
                  std::to_string(dark.g) + ';' +
                  std::to_string(dark.b) + 'm';
      }

        buffer += utils::calculateBraille(dots);

        prevbright = bright;
        prevdark = dark;
      }
      buffer += "\033[0m\n";
      prevbright = RGB();
      prevdark = RGB();
    }
    


  std::cout << buffer.c_str();

}

} // namespace utils
