#ifndef TRIANGLE_T_H
#define TRIANGLE_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Material_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Shapes {
    /**
     * @brief The triangle class defines a triangle shape that can be intersected by rays.
     * 
     * A triangle is defined by three points, in counter-clockwise order. Its transformation matrix is used 
     * to modify those points.
     */
    class Triangle_t final : public Shape_t{
        public:
            /**
             * @brief Construct a new Triangle_t object with a transformation matrix and material from three points.
             * 
             * @param material Material of the triangle. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the triangle to modify its points.
             * @param points Array of three points, in counter-clockwise order, defining the triangle.
             * @param normals Array of three normals, in counter-clockwise order, at the three points of the triangle.
             * @param texcoord Array of three texture coordinates with two components, in counter-clockwise order, at the three points of the triangle. [x0, y0, x1, y1, x2, y2]
             */
            Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, const Vec3f* points, const Vec3f* normals, const double* texcoord);

            /**
             * @brief Destroy the Triangle_t object. Does nothing.
             */
            virtual ~Triangle_t() final;

            Vec3f points_orig_[3];
            Vec3f normals_orig_[3];
            double texture_coordinates_[6];
            Vec3f points_[3];
            Vec3f normals_[3];
            Vec3f v0v1_;
            Vec3f v0v2_;
            double tuv_to_world_[2];
            Vec3f tangent_vec_; // Points up

            virtual void update() final;
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;
            virtual Vec3f normal_face(double time) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif