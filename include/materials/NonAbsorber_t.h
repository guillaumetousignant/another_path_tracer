#ifndef APTRACER_NONABSORBER_T_H
#define APTRACER_NONABSORBER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"

using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The non absorber class describes a medium that doesn't interact with light. 
     * 
     * This medium does nothing by itself, it is the identity medium.
     * This medium can be used to represent non interacting media like clear air or emptiness.
     */
    class NonAbsorber_t final : public Medium_t{
        public:
            /**
             * @brief Construct a new NonAbsorber_t object.
             * 
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority. 
             */
            NonAbsorber_t(double ind, unsigned int priority);

            /**
             * @brief Defines the interaction between a ray and the medium.
             * 
             * This medium doesn't interact with light, and as such a ray is not changed here.
             * 
             * @param ray Ray modified by the medium. Its colour and mask can be changed.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed. Never the case for a non absorber.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned. Always the case for a non absorber.
             */
            virtual auto scatter(Ray_t &ray) -> bool final; 
    };
}}
#endif