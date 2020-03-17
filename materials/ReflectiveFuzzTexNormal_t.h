#ifndef REFLECTIVEFUZZTEXNORMAL_T_H
#define REFLECTIVEFUZZTEXNORMAL_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

class Texture_t;

namespace APTracer::Materials {
    class ReflectiveFuzzTexNormal_t final : public Material_t{
        public:
            ReflectiveFuzzTexNormal_t(const Vec3f &emission_, Texture_t* texture, const Texture_t* normal_map, double order, double diffusivity);
            virtual ~ReflectiveFuzzTexNormal_t() final;

            Texture_t* texture_;
            Vec3f emission_;
            const Texture_t* normal_map_;
            double order_; // 1 to inf, 1 is flat, 2 is parabole,  higher looks like normal and inf is straight reflection.
            double diffusivity_; // between 0 and 1, portion of the hemisphere reflected to.
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif