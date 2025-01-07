#ifndef APTRACER_HELPERS_COLOURS_HPP
#define APTRACER_HELPERS_COLOURS_HPP

#include "entities/Vec3f.hpp"
#include <string>
#include <unordered_map>

namespace APTracer { namespace Helpers {
    /**
     * @brief This maps some predefined colours to colour names.
     *
     * This is used to reference colours in scenes without having to type colour values.
     */
    extern const std::unordered_map<std::string, Entities::Vec3f> colours;
}}
#endif
