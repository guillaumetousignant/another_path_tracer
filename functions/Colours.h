#ifndef COLOURS_H
#define COLOURS_H

#include <map> 
#include <string>
#include "Vec3f.h"

using APTracer::Entities::Vec3f;

namespace APTracer::Colours {
    extern const std::map<std::string, Vec3f> colours;
}
#endif