#include "RandomGenerator_t.h"

namespace APTracer { namespace Entities {
    std::random_device rd;
    std::mt19937 rng = std::mt19937(rd());
}}