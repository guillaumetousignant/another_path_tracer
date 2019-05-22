#include <iostream>
#include "RandomGenerator_t.h"
#include "ReadScene.h"

int main(int argc, char **argv){
    if (argc < 2){
        std::cout << "Must specify scene filename. Exiting." << std::endl;
        return 42;
    }

    read_scene(argv[1]);
    
    return 0;
}