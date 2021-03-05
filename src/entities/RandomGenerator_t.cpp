#include "entities/RandomGenerator_t.h"
#ifdef APTRACER_USE_OPENMP
#include <omp.h>
#endif

std::vector<std::mt19937> APTracer::Entities::build_rng_vec() {
    std::vector<std::mt19937> generators;
    std::random_device rd;

    #ifdef APTRACER_USE_OPENMP
    const int N = omp_get_max_threads();
    #else
    const int N = 1;
    #endif

    for (int i = 0; i < N; ++i) {
        generators.emplace_back(rd());
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