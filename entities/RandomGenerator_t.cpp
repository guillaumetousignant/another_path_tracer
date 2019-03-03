#include "RandomGenerator_t.h"

namespace my_rand{
    std::random_device rd;
    std::mt19937 rng = std::mt19937(rd());
}