#ifndef DISTANCEMATERIAL_T_H
#define DISTANCEMATERIAL_T_H

#include "entities/Material_t.h"
#include "entities/Ray_t.h"

namespace APTracer::Entities {
    class Shape_t;
}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer::Materials {
    class DistanceMaterial_t final : public Material_t{
        public:
            DistanceMaterial_t(double focal_length, double exponent);
            virtual ~DistanceMaterial_t() final;

            double focal_length_;
            double exponent_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif