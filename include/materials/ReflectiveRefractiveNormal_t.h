#ifndef REFLECTIVEREFRACTIVENORMAL_T_H
#define REFLECTIVEREFRACTIVENORMAL_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Medium_t;
    class Texture_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Materials {
    class ReflectiveRefractiveNormal_t final : public Material_t{
        public:
            ReflectiveRefractiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map, Medium_t* medium);
            virtual ~ReflectiveRefractiveNormal_t() final;

            Vec3f emission_;
            Vec3f colour_;
            const Texture_t* normal_map_;
            std::uniform_real_distribution<double> unif_;
            Medium_t* medium_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif