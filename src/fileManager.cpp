#include "fileManager.hpp"
#include <stdexcept>
#include "gif.hpp"
#include "image.hpp"
#include "video.hpp"
#include <iostream>
#include <string_view>
#include "utils.hpp"
namespace ext {
    FileManager::FileManager(const std::string &path, int argc, char * argv[]) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if(!file.is_open()) {
            throw std::runtime_error("could not open " + path);
        }
        utils::Options options(argc, argv);
        if(options.targetHeight == 1 && options.targetWidth == 1) {
            options.setFullScreen();
        }
        if(isGif(file)) {
            Gif gif(path, options);
            gif.renderGif();
        }
        else {
            if(path.ends_with(".mp4")) {
                  VideoDecoder video(path, options);
           video.renderVideo();
            } else {
            Image img(path, options);
            img.renderImage();
            
            }
        }

        
    }


    bool FileManager::isGif(std::ifstream &file) {

        file.seekg(0);

        char signature[6];
        
        
        file.read(signature, 6);


        std::string_view view(signature);
        return view == "GIF87a" || view == "GIF89a";
    }
}