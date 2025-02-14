#include "entities/RandomGenerator_t.hpp"
#ifdef APTRACER_USE_OPENMP
    #include <omp.h>
#endif

auto APTracer::Entities::build_rng_vec() -> std::vector<std::mt19937> {
    std::vector<std::mt19937> generators;
    std::random_device rdev;

#ifdef APTRACER_USE_OPENMP
    const int n_threads = omp_get_max_threads();
#else
    const int n_threads = 1;
#endif

    generators.reserve(n_threads);

    for (int i = 0; i < n_threads; ++i) {
        generators.emplace_back(rdev());
    }
    return generators;
}

std::vector<std::mt19937> APTracer::Entities::rng_vec = APTracer::Entities::build_rng_vec();

auto APTracer::Entities::rng() -> std::mt19937& {
#ifdef APTRACER_USE_OPENMP
    return rng_vec[omp_get_thread_num()];
#else
    return rng_vec[0];
#endif
}
