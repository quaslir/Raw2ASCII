#include "fileManager.hpp"
#include "gif.hpp"
#include "image.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>
#include <string>
int main(int argc, char *argv[]) {

  
  utils::Options opts(argc, argv);

  try {
  ext::FileManager manager(std::move(opts));
  } catch(const std::exception & err) {
    std::cerr << err.what() << '\n';
  }
  return 0;
}