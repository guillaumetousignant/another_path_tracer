#ifndef APTRACER_ENTITIES_MEDIUM_T_HPP
#define APTRACER_ENTITIES_MEDIUM_T_HPP

#include "entities/Ray_t.hpp"

namespace APTracer { namespace Entities {

    /**
     * @brief The medium class describes a medium in which a ray of light can travel, like water, glass or air.
     *
     * A medium is like a volumetric material. It can modify a ray of light depending on the distance the ray has passed
     * through it, contrary to a material which modifies a ray in a punctual manner. It has a refractive index to calculate
     * how the light changes direction when passing from a medium to another, and the reflection ratio of refractive surfaces.
     * It has a priority to permit the use of overlapping mediums. A material which passes a ray through a surface, such as
     * the surface of a glass ball, should add a medium to a ray's medium list to indicate that it is now travelling through it.
     * A ray can have multiple mediums in its list, the one with the highest priority is the current medium. Intersections with
     * lower priority mediums are ignored.
     */
    class Medium_t {
        public:
            /**
             * @brief Construct a new Medium_t object.
             *
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums.
             */
            Medium_t(double ind, unsigned int priority) : ind_(ind), priority_(priority){};

            /**
             * @brief Destroy the Medium_t object.
             */
            virtual ~Medium_t() = default;

            double ind_; /**< @brief Refractive index of the medium. Used to calculate a ray's change of direction when going from a medium to another, and reflection ratio of refractive surfaces.*/
            unsigned int priority_; /**< @brief Priority of the medium over other mediums. A ray's current medium is the one with the highest priority, and intersections with lower priority mediums
                                       are ignored when there is an overlap.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             *
             * This function describes how a ray of light is modified by moving through the medium. This can mean changing the ray's colour and mask to model light absorption and emission of the
             * medium. The function can also scatter rays. This means that the ray's direction and origin are changed by the medium, for example to model intersection with a particle. In this case,
             * the function must return true, to avoid resolving the ray-shape intersection, as the ray has changed direction and possibly doesn't hit the shape anymore. It is called after a hit has
             * been found, but before bouncing on the shape's material. This bounce is not performed if the ray is scattered, and the ray's distance should be changed accordingly.
             *
             * @param ray Ray modified by the medium. Its colour and mask can be changed, and its origin and destination can be changed if it is scattered.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned. Does not mean that the ray's colour and/or mask
             * haven't been modified.
             */
            virtual auto scatter(Ray_t& ray) -> bool = 0;
    };
}}

#endif