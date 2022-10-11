#ifndef APTRACER_GRIDCELLARRAY_T_H
#define APTRACER_GRIDCELLARRAY_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"
#include <vector>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell array class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in an array, for fast iterating and lowest memory use.
     * 
     * This structure with its array representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is highest. With arrays, iteration on
     * shapes is slightly faster, and the memory used is lowest. Slightly longer to build, needs two passes.
     */
    class GridCellArray_t final : public AccelerationStructure_t {
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
            GridCellArray_t(size_t size);

            size_t size_; /**< @brief Size of the array. Number of shapes that can be held by the cell without reallocating.*/
            std::vector<Shape_t*> items_; /**< @brief Array of shapes contained in the cell. This allows fast iterating and lowest memory use.*/
            size_t increment_size_; /**< @brief Size that will be allocated next time 'reserve()' is called. Increased with 'operator++'.*/

            virtual auto intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* final;
            virtual auto add(Shape_t* item) -> void final;
            virtual auto remove(const Shape_t* item) -> void final;
            virtual auto move(Shape_t* item) -> void final;
            virtual auto size() const -> size_t final;

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