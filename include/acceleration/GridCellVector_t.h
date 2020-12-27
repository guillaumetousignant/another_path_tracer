#ifndef APTRACER_GRIDCELLVECTOR_T_H
#define APTRACER_GRIDCELLVECTOR_T_H

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
     * @brief The grid cell vector class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in a vector, for fast iteraring and low memory use.
     * 
     * This structure with its vector representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is higher. With vectors, iteration on
     * shapes is slightly faster, and the memory used is low.
     */
    class GridCellVector_t final : public AccelerationStructure_t{
        public:
            /**
             * @brief Construct a new GridCellVector_t object with no shapes.
             */
            GridCellVector_t();

            /**
             * @brief Construct a new GridCellVector_t object capable of holding 'size' shapes.
             * 
             * @param size Initial reserved size of the shape vector.
             */
            GridCellVector_t(size_t size);

            /**
             * @brief Destroy the GridCellVector_t object, destroying its list of shapes.
             */
            virtual ~GridCellVector_t() final;

            std::vector<Shape_t*> items_; /**< @brief Vector of shapes contained in the cell. This allows fast iterating and low memory use.*/
            size_t size_; /**< @brief Size that will be allocated next time 'reserve()' is called. Increased with 'operator++'.*/

            virtual Shape_t* intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            virtual void move(Shape_t* item) final;

            /**
             * @brief Reserves 'size_' in the shape vector.
             */
            void reserve();

            /**
             * @brief Increase 'size_', to eventually reserve the new size of the vector with 'reserve()'.
             * 
             * @return GridCellVector_t& Reference to this, for chaining operations.
             */
            GridCellVector_t& operator++();
    };
}}
#endif