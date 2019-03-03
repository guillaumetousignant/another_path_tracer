#ifndef RANDOMGENERATOR_T_H
#define RANDOMGENERATOR_T_H

#include <random> 

class RandomGenerator_t {
    public:
        RandomGenerator_t();
        ~RandomGenerator_t();

        std::mt19937 rng_;
        std::uniform_real_distribution<double> unif_;

        double rand();
};
#endif