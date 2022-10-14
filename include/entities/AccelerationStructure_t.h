#ifndef APTRACER_ENTITIES_ACCELERATIONSTRUCTURE_T_H
#define APTRACER_ENTITIES_ACCELERATIONSTRUCTURE_T_H

#include "entities/Ray_t.h"
#include <array>

namespace APTracer { namespace Entities {
    class Shape_t;

    /**
     * @brief The acceleration structure class holds shapes and can be intersected.
     *
     * This class is what is queried to find what shape a ray intersects. It is an interface
     * class, defining what a concrete acceleration structure must implement. An acceleration
     * structure should accelerate ray intersection compared to a brute-force approach.
     */
    class AccelerationStructure_t {
        public:
            /**
             * @brief Intersects a ray with the acceleration structure to find the closest hit
             * and its characteristics.
             *
             * Main function of the acceleration structure. Virtual function, it should return
             * the information of the closest intersection, or nullptr if no intersection. t
             * and uv may have undefined values if no intersection is found.
             *
             * @param[in] ray Ray to be intersected with the acceleration structure, using its current origin and direction.
             * @param[out] t Distance to intersection. It is stored in t if there is an intersection.
             * @param[out] uv 2D object-space coordinates of the intersection.
             * @return Shape_t* Pointer of the intersected shape. Returns nullptr if there is no intersection.
             */
            virtual auto intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* = 0;

            /**
             * @brief Adds a shape to the acceleration structure.
             *
             * Virtual function to add a shape to the acceleration structure. Must be implemented to make multi-level
             * acceleration structures possible. Not all acceleration structures are capable of
             * inserting shapes without a refit, so this operation may be pricey and should be
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             *
             * @param[in] item Shape to be added to the acceleration structure.
             */
            virtual auto add(Shape_t* item) -> void = 0;

            /**
             * @brief Removes a shape from the acceleration structure.
             *
             * Virtual function to remove a shape from the acceleration structure. Must be implemented to make moving objects
             * possible. Not all acceleration structures are capable of
             * removing shapes without a refit, so this operation may be pricey and should be
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             *
             * @param item Shape to be removed from the acceleration structure.
             */
            virtual auto remove(const Shape_t* item) -> void = 0;

            /**
             * @brief Moves a shape in the acceleration structure.
             *
             * Virtual function to move a shape within the acceleration structure. Must be implemented to make moving objects
             * possible. Not all acceleration structures are capable of
             * moving shapes without a refit, so this operation may be pricey and should be
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             *
             * @param item Shape to be removed from the acceleration structure.
             */
            virtual auto move(Shape_t* item) -> void = 0;

            /**
             * @brief Number of shapes currently held in the acceleration structure.
             *
             * @return size_t Number of shapes currently held in the acceleration structure.
             */
            virtual auto size() const -> size_t = 0;
    };
}}

#endif