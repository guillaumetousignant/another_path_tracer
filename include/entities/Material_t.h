#ifndef MATERIAL_T_H
#define MATERIAL_T_H

#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;

    /**
     * @brief The material class describes how light interacts with a specific material.
     * 
     * This is an interface class that define the core function a material must implement.
     * A material modified a ray of light when it bounces on it. It can change the ray's
     * direction and origin, to model the light path change from the intersection. It can 
     * also change the ray's colour and mask to model light absorption and emission.
     */
    class Material_t{
        public:
            /**
             * @brief Construct a new Material_t object. Never called directly.
             */
            Material_t(){};

            /**
             * @brief Destroy the Material_t object. Does nothing.
             */
            virtual ~Material_t(){};

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * This is the main function to resolve intersection between a ray and a shape.
             * This is how rays get coloured and change direction after an intersection in the scene.
             * This sets the ray's new colour, mask, direction and origin.
             * 
             * @param uv Shape space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) = 0;
    };
}}

#endif