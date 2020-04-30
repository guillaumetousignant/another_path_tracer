#ifndef DIRECTIONALLIGHT_T_H
#define DIRECTIONALLIGHT_T_H

#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;

    /**
     * @brief The DirectionalLight_t class represents an infinitely angular light, with a direction, intensity and radius.
     * 
     * These can be used to check intersection agains a ray and colour it if hit.
     */
    class DirectionalLight_t{
        public:
            /**
             * @brief Construct a new DirectionalLight_t object
             * 
             * @param intensity Colour of the light. Can be any value.
             * @param transformation Transformation matrix used by the light. Sets the light's direction and radius when created and updated.
             */
            DirectionalLight_t(const Vec3f &intensity, TransformMatrix_t* transformation);

            /**
             * @brief Destroy the DirectionalLight_t object. Does nothing.
             */
            ~DirectionalLight_t();

            Vec3f direction_; /**< @brief Direction of the light. Defines towards where the light points. Changed by modifying the light's transformation matrix.*/
            Vec3f intensity_; /**< @brief Colour of the light. Defines the value that will be multiplied with a ray's mask to set the ray's colour on intersection.*/
            TransformMatrix_t* transformation_; /**< @brief Transformation matrix used by the light. Sets the light's direction and radius when created and updated.*/
            double radius_; /**< @brief Radius of the light. Sets the light's size, which is how far from the direction a ray can be and still hit the light.*/

            /**
             * @brief Updates the camera's members.
             * 
             * This changes the light's direction and radius as per the transformation matrix. The radius is changed by the transformation matrix's scale. This is how changes to the transformation matrix tak eeffect.
             */
            void update();
    };
}}

#endif