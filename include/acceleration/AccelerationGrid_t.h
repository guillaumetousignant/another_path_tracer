#ifndef ACCELERATIONGRID_T_H
#define ACCELERATIONGRID_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"
#include "entities/Vec3f.h"
#include "shapes/Box_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;
using APTracer::Shapes::Box_t;

namespace APTracer { namespace Acceleration {
    class GridCell_t;

    /**
     * @brief The acceleration grid class describes a grid acceleration structure, splitting the space into uniform cells. Cells use lists of shapes, for constant time adding and removal of shapes. 
     * 
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update, but makes them vulnerable to the "teapot in a stadium" problem, where a single cell can contain a disproportionate amount of geometry.
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span.
     * This structure with its list representation is more geared toward moving scenes, as the cost to remove, add or move shapes from one cell to another is lower.
     */
    class AccelerationGrid_t final : public AccelerationStructure_t{
        public:
            /**
             * @brief Construct a new AccelerationGrid_t object from an array of shapes.
             * 
             * @param items Array of shapes to be added to the acceleration structure.
             * @param n_items Number of shapes to be added to the acceleration structure.
             * @param coordinates Array of minimum and maximum coordinates covered by the acceleration structure. Defaults to nullptr, to calculate from shapes. Set to restrict span, for example to use as a cell within another grid.
             * @param level Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc. Defaults to 0.
             * @param min_res Minimum number of cells for all directions. Defaults to 1.
             * @param max_res Maximum number of cells for all directions. Defaults to 128.
             */
            AccelerationGrid_t(Shape_t** items, size_t n_items, const Vec3f* coordinates = nullptr, unsigned int level = 0, unsigned int min_res = 1, unsigned int max_res = 128);

            /**
             * @brief Destroy the AccelerationGrid_t object, destroying the cells it owns.
             */
            virtual ~AccelerationGrid_t() final;

            GridCell_t** cells_; /**< @brief Array of all the cells contained in the acceleration structure. Cells use lists of shapes, for constant time adding and removal of shapes.*/ 
            unsigned int cell_res_[3]; /**< @brief Number of cells in the x, y, and z direction.*/ 
            Vec3f cell_size_; /**< @brief Span of the cells in the x, y, and z direction.*/ 
            Box_t bounding_box_; /**< @brief Box representing the space encompassed by the grid.*/ 
            unsigned int level_; /**< @brief Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.*/
            unsigned int min_res_; /**< @brief Minimum number of cells for all directions.*/
            unsigned int max_res_; /**< @brief Maximum number of cells for all directions.*/

            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            virtual void move(Shape_t* item) final;
    };
}}
#endif