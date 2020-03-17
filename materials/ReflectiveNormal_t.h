#ifndef REFLECTIVENORMAL_T_H
#define REFLECTIVENORMAL_T_H

#include "Material_t.h"
#include "Vec3f.h"

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

class Texture_t;

namespace APTracer::Materials {
    class ReflectiveNormal_t final : public Material_t{
        public:
            ReflectiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map);
            virtual ~ReflectiveNormal_t() final;

            Vec3f emission_;
            Vec3f colour_;
            const Texture_t* normal_map_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif