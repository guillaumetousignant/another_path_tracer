#ifndef APTRACER_ENTITIES_ACCELERATIONSTRUCTURE_T_HPP
#define APTRACER_ENTITIES_ACCELERATIONSTRUCTURE_T_HPP

#include "entities/Ray_t.hpp"
#include "entities/Shape_t.hpp"
#include <array>
#include <memory>

namespace APTracer { namespace Entities {

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
             * @brief Destroy the AccelerationStructure_t object.
             */
            virtual ~AccelerationStructure_t() = default;

            /**
             * @brief Construct a new AccelerationStructure_t object.
             */
            AccelerationStructure_t() = default;

            /**
             * @brief Copy construct a new AccelerationStructure_t object.
             *
             * @param other Object to copy.
             */
            AccelerationStructure_t(const AccelerationStructure_t& other) = default;

            /**
             * @brief Move construct a new AccelerationStructure_t object.
             *
             * @param other Object to move.
             */
            AccelerationStructure_t(AccelerationStructure_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return AccelerationStructure_t& Reference to this object.
             */
            auto operator=(const AccelerationStructure_t& other) -> AccelerationStructure_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return AccelerationStructure_t& Reference to this object.
             */
            auto operator=(AccelerationStructure_t&& other) noexcept -> AccelerationStructure_t& = default;

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
            virtual auto remove(Shape_t* item) -> void = 0;

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

            /**
             * @brief Returns a copy of the acceleration structure.
             *
             * Virtual function to clone an acceleration structure,containing the same shapes.
             * This is useful for recursively copying an acceleration structure when copying them.
             *
             * @return std::unique_ptr<AccelerationStructure_t>
             */
            virtual auto clone() const -> std::unique_ptr<AccelerationStructure_t> = 0;
    };
}}

#endif
