#include "RandomGenerator_t.h" // was never able to get this to work

#include <iostream> // REMOVE

extern RandomGenerator_t* rng;

int main(){
    RandomGenerator_t* rng = new RandomGenerator_t();
    std::cout << rng->rand();

    return 0;
}