#ifndef BOX_T_H
#define BOX_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Shapes {
    /**
     * @brief The box class describes an axis-aligned bounding bounding box.
     * 
     * This is meant to encompass other shapes, meshes or acceleration structures. It is described by two points
     * describing the maximum extent in all three coordinates and the minimum.
     */
    class Box_t {
        public:
            /**
             * @brief Construct a new Box_t object with [0, 0, 0] as both extents.
             */
            Box_t();

            /**
             * @brief Construct a new Box_t object with the provided minimum and maximum coordinates.
             * 
             * @param coord Array of [minimum, maximum] coordinates in the three axis for the bounding box.
             */
            Box_t(Vec3f (&coord)[2]);

            /**
             * @brief Destroy the Box_t object. Does nothing.
             */
            ~Box_t();

            Vec3f coordinates_[2]; /**< @brief Array of [minimum, maximum] coordinates in the three axis for the bounding box.*/

            /**
             * @brief Tests a ray for intersection with the bounding box, setting the intersection distance.
             * 
             * @param[in] ray Ray to test intersection against the bounding box, according to its position and direction.
             * @param[out] t Distance from the ray origin to the intersection point. Set only if intersected, otherwise undefined.
             * @return true The ray intersects the bounding box. t is set to the intersection distance.
             * @return false The ray doesn't intersect the bounding box. t is undefined.
             */
            bool intersection(const Ray_t &ray, double &t) const;
    };
}}
#endif