#ifndef APTRACER_COLOURS_H
#define APTRACER_COLOURS_H

#include <unordered_map> 
#include <string>
#include "entities/Vec3f.h"

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