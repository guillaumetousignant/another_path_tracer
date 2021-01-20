#include "entities/RandomGenerator_t.h"
#ifdef APTRACER_USE_OPENMP
#include <omp.h>
#endif

std::random_device rd;

std::vector<std::mt19937> APTracer::Entities::build_rng_vec() {
    std::vector<std::mt19937> generators;
    #ifdef APTRACER_USE_OPENMP
    for (int i = 0, N = omp_get_max_threads(); i < N; ++i) {
    #else
    for (int i = 0, N = 1; i < N; ++i) {
    #endif
        generators.emplace_back(std::mt19937(rd()));
    }
    return generators;
}

std::vector<std::mt19937> APTracer::Entities::rng_vec = APTracer::Entities::build_rng_vec();

std::mt19937& APTracer::Entities::rng() {
    #ifdef APTRACER_USE_OPENMP
    return rng_vec[omp_get_thread_num()];
    #else
    return rng_vec[0];
    #endif
}