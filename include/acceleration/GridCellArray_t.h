#ifndef GRIDCELLARRAY_T_H
#define GRIDCELLARRAY_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell array class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in an array, for fast iteraring and lowest memory use.
     * 
     * This structure with its array representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is highest. With arrays, iteration on
     * shapes is slightly faster, and the memory used is lowest. Slightly longer to build, needs two passes.
     */
    class GridCellArray_t final : public AccelerationStructure_t{
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

            /**
             * @brief Destroy the GridCellArray_t object, destroying its array of shapes.
             */
            virtual ~GridCellArray_t() final;

            size_t size_; /**< @brief Size of the array. Number of shapes that can be held by the cell without reallocating.*/
            Shape_t** items_; /**< @brief Array of shapes contained in the cell. This allows fast iterating and lowest memory use.*/
            size_t increment_size_; /**< @brief Size that will be allocated next time 'reserve()' is called. Increased with 'operator++'.*/

            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            virtual void move(Shape_t* item) final;

            /**
             * @brief Increases 'size_' to 'increment_size_', allocating the array with a new size and transfering existing shapes.
             */
            void reserve();

            /**
             * @brief Increase 'increment_size_', to eventually reserve the new size of the array with 'reserve()'.
             * 
             * @return GridCellArray_t& Reference to this, for chaining operations.
             */
            GridCellArray_t& operator++();
    };
}}
#endif