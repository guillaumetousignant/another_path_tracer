#include "entities/RandomGenerator_t.h"

std::random_device rd;
std::mt19937 APTracer::Entities::rng = std::mt19937(rd());