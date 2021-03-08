#ifndef APTRACER_REFLECTIVENORMAL_T_H
#define APTRACER_REFLECTIVENORMAL_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Texture_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Materials {
    class ReflectiveNormal_t final : public Material_t {
        public:
            ReflectiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map);

            Vec3f emission_;
            Vec3f colour_;
            const Texture_t* normal_map_;

            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) -> void final;
    };
}}
#endif