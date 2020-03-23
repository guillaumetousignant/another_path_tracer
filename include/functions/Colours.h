#ifndef COLOURS_H
#define COLOURS_H

#include <map> 
#include <string>
#include "entities/Vec3f.h"

using APTracer::Entities::Vec3f;

namespace APTracer { namespace Colours {
    extern const std::map<std::string, Vec3f> colours;
}}
#endif