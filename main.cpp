#include <iostream>
#include <string>
#include "RandomGenerator_t.h"
#include "SceneContext_t.h"

int main(int argc, char **argv){
    std::string filename = "";
    if (argc < 2){
        std::cout << "Enter scene path:" << std::endl;
        while (filename.empty()){
            std::getline(std::cin, filename);
        }
        if (filename == "q"){
            return -1;
        }
    }
    else{
        filename = argv[1];
    }

    SceneContext_t scene_context = SceneContext_t();

    scene_context.readXML(filename);
    scene_context.render();
    
    return 0;
}