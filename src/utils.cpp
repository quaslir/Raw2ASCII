#include "utils.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
namespace utils {

Options::Options(int argc,char *argv[]) {
  setFullScreen();
  parse(argc, argv);
}
void Options::parse(int argc,char *argv[]) {

  if (argc < 2) {
    readStdin = true;
    return;
  }

  for (int i = 1; i < argc; i++) {
    std::string_view param(argv[i]);

    if (param.starts_with("--help")) {
      help = true;
      return;
    }

    if ((param.starts_with("-w")) || (param.starts_with("--width="))) {
      size_t index = param.starts_with("-w") ? std::string{"-w"}.length()
                                             : std::string{"--width"}.length();
      try {
        int w = std::stoi(std::string{param.substr(index)});
        targetWidth = w;
      } catch (...) {
        throw std::invalid_argument("invalid value");
      }
    }

    else if ((param.starts_with("-h")) || (param.starts_with("--height="))) {
      size_t index = param.starts_with("-h")
                         ? std::string{"-h"}.length()
                         : std::string{"--height="}.length();
      try {
        int h = std::stoi(std::string{param.substr(index)});
        targetHeight = h;
      } catch (...) {
        throw std::invalid_argument("invalid value");
      }
    }

    else if (param.starts_with("-f") || param.starts_with("--fps")) {
      fps = true;
    }

    else if ((param.starts_with("-o")) || (param.starts_with("--output="))) {
      size_t index = param.starts_with("-o")
                         ? std::string{"-o"}.length()
                         : std::string{"--output="}.length();
      outputPath = param.substr(index);
    }

    else if (param == "--braille" || param == "-b") {
      braille = true;
    } else if (param.starts_with("-t") || param.starts_with("--threshold=")) {
      size_t index = param.starts_with("-t")
                         ? std::string{"-t"}.length()
                         : std::string{"--threshold="}.length();
      try {
        int th = std::stoi(std::string{param.substr(index)});
        threshold = th;
      } catch (...) {
        throw std::invalid_argument("invalid value");
      }
    } else {
      if (param == "-") {
        readStdin = true;
      } else
        file = param;
    }
  }

  if (file.empty()) {
    readStdin = true;
  }
}

void Options::setFullScreen(void) {
  struct winsize window;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
  targetWidth = window.ws_col * 0.8;
  targetHeight = window.ws_row;
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

std::string Options::renderBraille(const std::vector<RGB> &frame) const {
  std::string buffer;
  buffer += "\033[H";

  for (int y = 0; y < targetHeight; y += 4) {
    RGB prevbright, prevdark;

    for (int x = 0; x < targetWidth; x += 2) {

      RGB dark, bright;
      int minLum = 256;
      int maxLum = -1;
      for (int dy = 0; dy < 4; dy++) {
        for (int dx = 0; dx < 2; dx++) {

          int currentX = dx + x;
          int currentY = dy + y;

          if (currentX < targetWidth && currentY < targetHeight) {

            const RGB &p = frame[(y + dy) * targetWidth + (x + dx)];

            int lum = (p.r * 213 + p.g * 715 + p.b * 72) / 1000;
            if (lum < minLum) {
              minLum = lum;
              dark = p;
            }
            if (lum > maxLum) {
              maxLum = lum;
              bright = p;
            }
          }
        }
      }

      int contrast = maxLum - minLum;
      int threshold = (minLum + maxLum) / 2;
      bool dots[2][4] = {0};
      if (contrast > 25) {
        for (int dy = 0; dy < 4; dy++) {
          for (int dx = 0; dx < 2; dx++) {

            int currentX = dx + x;
            int currentY = dy + y;

            if (currentX < targetWidth && currentY < targetHeight) {
              const RGB &p = frame[currentY * targetWidth + currentX];
              int pLum = (p.r * 213 + p.g * 715 + p.b * 72) / 1000;

              dots[dx][dy] = (pLum > threshold);
            }
          }
        }

      } else {
        bright = dark;
      }
      if (bright != prevbright) {
        buffer +=
            "\033[38;2;" + std::to_string(bright.r) + ';' + std::to_string(bright.g) + ';' + std::to_string(bright.b) + 'm';
      }

      if (dark != prevdark) {
        buffer += "\033[48;2;" + std::to_string(dark.r) + ';' + std::to_string(dark.g) + ';' + std::to_string(dark.b)+ 'm';
      }

      buffer += utils::calculateBraille(dots);

      prevbright = bright;
      prevdark = dark;
    }
    buffer += "\033[0m\n";
    prevbright = RGB();
    prevdark = RGB();
  }

  return buffer;
}

std::vector<char> readStdin(void) {
  return std::vector<char>((std::istreambuf_iterator<char>(std::cin)),
                           std::istreambuf_iterator<char>());
}

std::vector<char> readFile(const std::string &file) {
  std::ifstream f(file, std::ios::binary | std::ios::ate);

  if (!f.is_open()) {
    throw std::invalid_argument("could not read file");
  }

  std::streamsize size = f.tellg();
  f.seekg(0, std::ios::beg);

  std::vector<char> data(size);

  if (f.read(data.data(), size)) {
    return data;
  }
  return {};
}

void Options::writeFile(std::string &&buffer) const {
  std::ofstream targetFile(outputPath, std::ios::app);
  targetFile << buffer;
}

bool isSimilar(const RGB &p1, const RGB &p2, int th) {
  int dr = p1.r - p2.r;
  int dg = p1.g - p2.g;
  int db = p1.b - p2.b;

  return (dr * dr + dg * dg + db * db) < (th * th);
}
} // namespace utils
