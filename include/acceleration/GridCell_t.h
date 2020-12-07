#ifndef APTRACER_GRIDCELL_T_H
#define APTRACER_GRIDCELL_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"
#include <list>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in a list, for constant time adding and removal of shapes.
     * 
     * This structure with its list representation is more geared toward moving scenes, as the cost to remove, add or move shapes from one cell to another is lower.
     */
    class GridCell_t final : public AccelerationStructure_t{
        public:
            /**
             * @brief Construct a new GridCell_t object containing no shapes.
             */
            GridCell_t();

            /**
             * @brief Destroy the GridCell_t object, destroying its list of shapes.
             */
            virtual ~GridCell_t() final;

            std::list<Shape_t*> items_; /**< @brief List of shapes contained in the cell. This allows constant time adding and removal of shapes.*/

            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            virtual void move(Shape_t* item) final;
    };
}}
#endif