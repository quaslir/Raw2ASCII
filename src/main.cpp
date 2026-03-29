#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "utils.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <string>
int main(int argc, char *argv[]) {
  std::signal(SIGINT, utils::restoreTerminal);
  auto help = R"(Usage: RAW2ASCII [file] [options] 
A utility to convert raw video/image data into ASCII or Braille art.
Arguments:
  file                  Input file path (default: reads from stdin)
  -                     Read input from stdin
Options:
  --help            Show this help message and exit
  -w, --width=VALUE     Set the target width for the output
  -h, --height=VALUE        Set the target height for the output (Note: -h is help)
  -f, --fps             Enable FPS limiting/sync (matches video source)
  -o, --output=PATH     Save the output to a specific file instead of stdout
  -b, --braille         Use Braille characters for rendering (higher density)
  -t, --threshold=VALUE Set similarity threshold for frame optimization (0-255)
  -8	--8bit	Use xterm-256color (8-bit) rendering mode)";

  try {
    utils::Options opts;
    opts.parse(argc, argv);
    if (opts.help) {
      std::cout << help << std::endl;
      return 0;
    }
    ext::FileManager manager(std::move(opts));
    manager.open();
  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
    utils::restoreTerminal(0);
  }
  return 0;
}