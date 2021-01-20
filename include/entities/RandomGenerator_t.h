#ifndef APTRACER_RANDOMGENERATOR_T_H
#define APTRACER_RANDOMGENERATOR_T_H

#include <random>
#include <vector>

namespace APTracer { namespace Entities {
    extern std::vector<std::mt19937> rng_vec;
    std::vector<std::mt19937> build_rng_vec();
    std::mt19937& rng(); /**< @brief Global random generator initialised once per thread and used through the program.*/
}}
#endif