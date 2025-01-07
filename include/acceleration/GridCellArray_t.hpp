#ifndef APTRACER_GRIDCELLARRAY_T_HPP
#define APTRACER_GRIDCELLARRAY_T_HPP

#include "entities/AccelerationStructure_t.hpp"
#include <vector>

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell array class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in an array, for fast iterating and lowest memory use.
     *
     * This structure with its array representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is highest. With arrays, iteration on
     * shapes is slightly faster, and the memory used is lowest. Slightly longer to build, needs two passes.
     */
    class GridCellArray_t final : public Entities::AccelerationStructure_t {
        public:
            /**
             * @brief Construct a new GridCellArray_t object with no shapes.
             */
            GridCellArray_t();

            /**
             * @brief Construct a new GridCellArray_t object capable of holding 'size' shapes.
             *
             * @param size Initial reserved size of the shape array.
             */
            explicit GridCellArray_t(size_t size);

            size_t size_; /**< @brief Size of the array. Number of shapes that can be held by the cell without reallocating.*/
            std::vector<Entities::Shape_t*> items_; /**< @brief Array of shapes contained in the cell. This allows fast iterating and lowest memory use.*/
            size_t increment_size_; /**< @brief Size that will be allocated next time 'reserve()' is called. Increased with 'operator++'.*/

            auto intersect(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Entities::Shape_t* final;
            auto add(Entities::Shape_t* item) -> void final;
            auto remove(Entities::Shape_t* item) -> void final;
            auto move(Entities::Shape_t* item) -> void final;
            auto size() const -> size_t final;
            auto clone() const -> std::unique_ptr<AccelerationStructure_t> final;

            /**
             * @brief Increases 'size_' to 'increment_size_', allocating the array with a new size and transferring existing shapes.
             */
            auto reserve() -> void;

            /**
             * @brief Increase 'increment_size_', to eventually reserve the new size of the array with 'reserve()'.
             *
             * @return GridCellArray_t& Reference to this, for chaining operations.
             */
            auto operator++() -> GridCellArray_t&;
    };
}}
#endif
