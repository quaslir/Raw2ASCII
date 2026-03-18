
#include <string>
#include <fstream>

namespace ext {

struct FileManager {


    FileManager(const std::string & path, int argc, char * argv[]);

    bool isGif(std::ifstream & file);

};

}