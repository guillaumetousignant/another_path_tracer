#ifndef APTRACER_MATERIALS_PORTALSCATTERER_T_H
#define APTRACER_MATERIALS_PORTALSCATTERER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"
#include <list>
#include <random>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
}}

using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The portal scatterer class describes a medium that transforms rays when scattering them, effectively moving them to another location.
     *
     * This medium can move rays from their hit point to another location using a transformation matrix. Their medium list
     * is overwritten with the material's, to represent their new position medium hierarchy.
     * It moves the rays when they are scattered by the medium, according to its scattering coefficient.
     * This medium can represent volumetric portals, impossible geometry, or overlaid images.
     * It is one-way, as there is no linked object at the outlet of the portal to be intersected.
     */
    class PortalScatterer_t final : public Medium_t {
        public:
            /**
             * @brief Construct a new PortalScatterer_t object with a transformation matrix, a medium list, and a scattering distance.
             *
             * @param transformation Transformation matrix which transforms the rays on hit, potentially moving them to another location.
             * @param medium_list List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param scattering_distance Average distance at which a ray is scattered. Inverse of the scattering coefficient.
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority.
             */
            PortalScatterer_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list, double scattering_distance, double ind, unsigned int priority);

            TransformMatrix_t* transformation_; /**< @brief Transformation matrix which transforms the rays when scattered, potentially moving them to another location.*/
            std::list<Medium_t*>
                medium_list_; /**< @brief List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).*/
            double scattering_coefficient_; /**< @brief Scattering coefficient, inverse of the average scattering distance. A higher coefficient will Have more chance to scatter rays.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             *
             * The ray has a chance of being scattered by the material depending on the material's scattering coefficient.
             * If scattered, the rays' position and direction are transformed to potentially move and/or rotate them to another location.
             * Their medium list is overwritten with the material's, to represent their new position medium hierarchy.
             * The scattering probability is (1 - e^(−t⋅σ_t)), where t is the distance to next intersection and σ_t is the scattering
             * coefficient. This means that the scattering probability is 0 at 0 traveled distance, and 1 at infinity traveled distance.
             * A higher scattering coefficient increases the probability between these bounds.
             *
             * @param ray Ray modified by the medium. Its position and direction can be changed.
             * @return Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned.
             */
            virtual auto scatter(Ray_t& ray) -> bool final;
    };
}}
#endif