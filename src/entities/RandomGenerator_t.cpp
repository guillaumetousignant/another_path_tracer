#include "entities/RandomGenerator_t.h"
#include <omp.h>

std::random_device rd;

std::vector<std::mt19937> APTracer::Entities::build_rng_vec() {
    std::vector<std::mt19937> generators;
    for (int i = 0, N = omp_get_max_threads(); i < N; ++i) {
        generators.emplace_back(std::mt19937(rd()));
    }
    return generators;
}

std::vector<std::mt19937> APTracer::Entities::rng_vec = APTracer::Entities::build_rng_vec();

std::mt19937& APTracer::Entities::rng() {
    return rng_vec[omp_get_thread_num()];
}