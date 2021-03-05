#ifndef APTRACER_ACCELERATIONGRIDARRAY_T_H
#define APTRACER_ACCELERATIONGRIDARRAY_T_H

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
    class GridCellArray_t;

    /**
     * @brief The acceleration grid array class describes a grid acceleration structure, splitting the space into uniform cells. Cells use arrays of shapes, for fast iteraring and lowest memory use.
     * 
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update, but makes them vulnerable to the "teapot in a stadium" problem, where a single cell can contain a disproportionate amount of geometry.
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span.
     * This structure with its array representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is highest. With arrays, iteration on
     * shapes is slightly faster, and the memory used is lowest. Slightly longer to build, needs two passes.
     */
    class AccelerationGridArray_t final : public AccelerationStructure_t{
        public:
            /**
             * @brief Construct a new top-level AccelerationGridArray_t object from an array of shapes.
             * 
             * @param items Array of shapes to be added to the acceleration structure.
             * @param n_items Number of shapes to be added to the acceleration structure.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             */
            AccelerationGridArray_t(Shape_t** items, size_t n_items, size_t min_res, size_t max_res);

            /**
             * @brief Construct a new AccelerationGridArray_t object from an array of shapes.
             * 
             * @param items Array of shapes to be added to the acceleration structure.
             * @param n_items Number of shapes to be added to the acceleration structure.
             * @param coordinates Array of minimum and maximum coordinates covered by the acceleration structure. Set to restrict span, for example to use as a cell within another grid.
             * @param level Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             */
            AccelerationGridArray_t(Shape_t** items, size_t n_items, std::array<Vec3f, 2> coordinates, unsigned int level, size_t min_res, size_t max_res);

            /**
             * @brief Destroy the AccelerationGridArray_t object, destroying the cells it owns.
             */
            virtual ~AccelerationGridArray_t() final;

            GridCellArray_t** cells_; /**< @brief Array of all the cells contained in the acceleration structure. Cells use arrays of shapes, for fast iteraring and lowest memory use.*/
            std::array<size_t, 3> cell_res_; /**< @brief Number of cells in the x, y, and z direction.*/
            Vec3f cell_size_; /**< @brief Span of the cells in the x, y, and z direction.*/
            Box_t bounding_box_; /**< @brief Box representing the space encompassed by the grid.*/
            unsigned int level_; /**< @brief Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.*/
            size_t min_res_; /**< @brief Minimum number of cells for all directions.*/
            size_t max_res_; /**< @brief Maximum number of cells for all directions.*/

            virtual auto intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* final;
            virtual auto add(Shape_t* item) -> void final;
            virtual auto remove(const Shape_t* item) -> void final;
            virtual auto move(Shape_t* item) -> void final;
    };
}}
#endif