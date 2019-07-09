#include <iostream>
#include "RandomGenerator_t.h"
#include "SceneContext_t.h"

int main(int argc, char **argv){
    if (argc < 2){
        std::cout << "Must specify scene filename. Exiting." << std::endl;
        return 42;
    }

    SceneContext_t scene_context = SceneContext_t();

    scene_context.readXML(argv[1]);
    scene_context.render();
    
    return 0;
}