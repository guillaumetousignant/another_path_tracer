#include <random> // was never able to get this to work

#include <iostream> // REMOVE

extern std::mt19937 rng;

int main(){
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<double> unif(0, 1);

    std::cout << unif(rng);

    return 0;
}