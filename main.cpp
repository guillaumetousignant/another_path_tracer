#include "RandomGenerator_t.h"

#include <iostream> // REMOVE

int main(){
    std::uniform_real_distribution<double> unif_(0, 1);

    std::cout << unif_(my_rand::rng) << std::endl;

    return 0;
}