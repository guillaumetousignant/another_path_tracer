#ifndef APTRACER_ENTITIES_RANDOMGENERATOR_T_H
#define APTRACER_ENTITIES_RANDOMGENERATOR_T_H

#include <random>
#include <vector>

namespace APTracer { namespace Entities {
    extern std::vector<std::mt19937> rng_vec; /**< @brief Global vector of random generators initialised once per thread and used through the program.*/

    /**
     * @brief Initialises the array of random generators, one for each thread.
     * 
     * This is called sometimes before main, when rng_vec is initialised in the global namespace. 
     * 
     * @return std::vector<std::mt19937> 
     */
    auto build_rng_vec() -> std::vector<std::mt19937>;

    /**
     * @brief Returns the thread's random generator within rng_vec.
     * 
     * @return std::mt19937& Reference to this thread's random generator.
     */
    auto rng() -> std::mt19937&;
}}
#endif