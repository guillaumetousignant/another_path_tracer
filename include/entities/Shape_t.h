#ifndef SHAPE_T_H
#define SHAPE_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Material_t;
    class TransformMatrix_t;

    /**
     * @brief The shape interface class describes surfaces which can be intersected by a ray.
     * 
     * Shapes are surfaces that can be intersected by rays and shaded. Rays bounce on them according to their material, and they can provide 
     * information about an intersection, like the hit point, object coordinates hit point, surface normal, etc. Shapes are stored in a scene and 
     * in an acceleration structure, and as such must provide the mincoord and maxcoord functions to be spatially sorted. The shapes can be modified 
     * by applying transformations to their transformation matrix. These changes are propagated on update.
     */
    class Shape_t{
        public:
            /**
             * @brief Construct a new Shape_t object with a material and a transformation matrix.
             * 
             * @param material Material of the shape.
             * @param transform_matrix Transformation matrix used to transform and move the shape.
             */
            Shape_t(Material_t *material, TransformMatrix_t *transform_matrix): material_(material), transformation_(transform_matrix){};

            /**
             * @brief Destroy the Shape_t object. Does nothing.
             */
            virtual ~Shape_t(){};

            /**
             * @brief Propagates changes from the shape's transformation matrix to the shape.
             * 
             * Usually called once per frame, will modify the shape once with all changes that have been made to the transformation
             * matrix since last update.
             */
            virtual void update() = 0;

            /**
             * @brief 
             * 
             * @param ray 
             * @param[out] t 
             * @param[out] uv 
             * @return true 
             * @return false 
             */
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const = 0;
            virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const = 0;
            virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const = 0;
            virtual void normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const = 0;
            virtual Vec3f mincoord() const = 0;
            virtual Vec3f maxcoord() const = 0;

            Material_t *material_;
            TransformMatrix_t *transformation_;
    };
}}
#endif