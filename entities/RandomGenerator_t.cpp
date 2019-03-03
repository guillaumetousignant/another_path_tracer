#include "RandomGenerator_t.h"

RandomGenerator_t::RandomGenerator_t() {
    std::random_device rd;
    rng_ = std::mt19937(rd());
    unif_ = std::uniform_real_distribution<double>(0, 1);
}

RandomGenerator_t::~RandomGenerator_t() {

}

double RandomGenerator_t::rand(){
    return unif_.operator()(rng_);
}