#ifndef APTRACER_PORTAL_T_H
#define APTRACER_PORTAL_T_H

#include "PortalTop_t.h"
#include "entities/Ray_t.h"
#include <list>

namespace APTracer { namespace Entities {
    class Shape_t;
    class TransformMatrix_t;
    class Medium_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Medium_t;

namespace APTracer { namespace Materials {
    class Portal_t final : public PortalTop_t{
        public:
            Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
            virtual ~Portal_t() final;

            virtual void bounce(const std::array<double, 2> &uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif