#include <ctime>
#include <random>

std::mt19937 rng;

int main(){
    rng.seed(std::random_device()());

    return 0;
}