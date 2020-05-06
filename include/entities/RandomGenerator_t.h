#ifndef RANDOMGENERATOR_T_H
#define RANDOMGENERATOR_T_H

#include <random> 

using std::mt19937;

namespace APTracer { namespace Entities {
    extern mt19937 rng; /**< @brief Global random generator initialised once per thread and used through the program.*/
}}
#endif