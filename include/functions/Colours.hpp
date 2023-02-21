#ifndef APTRACER_FUNCTIONS_COLOURS_HPP
#define APTRACER_FUNCTIONS_COLOURS_HPP

#include "entities/Vec3f.hpp"
#include <string>
#include <unordered_map>

using APTracer::Entities::Vec3f;

namespace APTracer { namespace Colours {
    /**
     * @brief This maps some predefined colours to colour names.
     *
     * This is used to reference colours in scenes without having to type colour values.
     */
    extern const std::unordered_map<std::string, Vec3f> colours;
}}
#endif