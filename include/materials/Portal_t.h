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
    
    /**
     * @brief The portal class describes a material that transforms rays on hit, effectively moving them to another location.
     * 
     * This material can move rays from their hit point to another location using a transformation matrix. Their medium list
     * is overwritten with the material's, to represent their new position medium hierarchy.
     * This material can represent portals, screens showing another area, or impossible geometry.
     * It is one-way, as there is no linked object at the outlet of the portal to be intersected.
     */
    class Portal_t final : public PortalTop_t{
        public:
            /**
             * @brief Construct a new Portal_t object with a transformation matrix and a medium list.
             * 
             * @param transformation Transformation matrix which transforms the rays on hit, potentially moving them to another location. 
             * @param medium_list  List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             */
            Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);

            /**
             * @brief Destroy the Portal_t object. Does nothing.
             */
            virtual ~Portal_t() final;

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * On intersection, the rays' position and direction are transformed to potentially move and/or rotate them to another location.
             * Their medium list is overwritten with the material's, to represent their new position medium hierarchy.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif