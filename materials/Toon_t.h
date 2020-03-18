#ifndef TOON_T_H
#define TOON_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include "Ray_t.h"

namespace APTracer::Entities {
    class Shape_t;
}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer::Materials {
    class Toon_t final : public Material_t{
        public:
            Toon_t(const Vec3f &colour);
            virtual ~Toon_t() final;

            Vec3f colour_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif