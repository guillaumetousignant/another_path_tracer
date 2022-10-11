#ifndef APTRACER_ACCELERATIONMULTIGRIDVECTOR_T_H
#define APTRACER_ACCELERATIONMULTIGRIDVECTOR_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"
#include "entities/Vec3f.h"
#include "shapes/Box_t.h"
#include <vector>
#include <memory>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;
using APTracer::Shapes::Box_t;

namespace APTracer { namespace Acceleration {
    /**
     * @brief The acceleration multi grid vector class describes a multi-level grid acceleration structure, splitting the space into uniform cells or grids, depending on the number of shapes in a cell. Cells use vectors of shapes, for fast iterating and low memory use.
     * 
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update. Multi level grids enable cells containing several shapes to be changed to grids. This can help alleviate the "Teapot in a stadium problem".
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span. Input parameters can set the number of shapes before a subgrid is created, and the maximum
     * grid recursion level.
     * This structure with its vector representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is higher. With vectors, iteration on
     * shapes is slightly faster, and the memory used is low.
     */
    class AccelerationMultiGridVector_t final : public AccelerationStructure_t {
        public:
            /**
             * @brief Construct a new top-level AccelerationMultiGridVector_t object from an array of shapes.
             * 
             * @param items Array of shapes to be added to the acceleration structure.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             * @param max_cell_content Maximum number of shapes in a cell before it is changed to a subgrid.
             * @param max_grid_level Maximum recursion level of the grid. 0 is no subgrids, 1 means cells from the top-level grid can be single level grids, etc.
             */
            AccelerationMultiGridVector_t(const std::vector<Shape_t*>& items, size_t min_res, size_t max_res, size_t max_cell_content, unsigned int max_grid_level);
           
            /**
             * @brief Construct a new AccelerationMultiGridVector_t object from an array of shapes.
             * 
             * @param items Array of shapes to be added to the acceleration structure.
             * @param coordinates Array of minimum and maximum coordinates covered by the acceleration structure. Set to restrict span, for example to use as a cell within another grid.
             * @param level Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             * @param max_cell_content Maximum number of shapes in a cell before it is changed to a subgrid.
             * @param max_grid_level Maximum recursion level of the grid. 0 is no subgrids, 1 means cells from the top-level grid can be single level grids, etc.
             */
            AccelerationMultiGridVector_t(const std::vector<Shape_t*>& items, std::array<Vec3f, 2> coordinates, unsigned int level, size_t min_res, size_t max_res, size_t max_cell_content, unsigned int max_grid_level);

            std::vector<std::unique_ptr<AccelerationStructure_t>> cells_; /**< @brief Array of all the cells and grids contained in the acceleration structure. Cells use vectors of shapes, for fast iterating and low memory use.*/
            std::array<size_t, 3> cell_res_; /**< @brief Number of cells in the x, y, and z direction.*/ 
            Vec3f cell_size_; /**< @brief Span of the cells in the x, y, and z direction.*/ 
            Box_t bounding_box_; /**< @brief Box representing the space encompassed by the grid.*/ 
            unsigned int level_; /**< @brief Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.*/
            size_t min_res_; /**< @brief Minimum number of cells for all directions.*/
            size_t max_res_; /**< @brief Maximum number of cells for all directions.*/
            size_t max_cell_content_; /**< @brief Maximum number of shapes in a cell before it is changed to a subgrid.*/
            unsigned int max_grid_level_; /**< @brief Maximum recursion level of the grid. 0 is no subgrids, 1 means cells from the top-level grid can be single level grids, etc.*/

            virtual auto intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* final;
            virtual auto add(Shape_t* item) -> void final;
            virtual auto remove(const Shape_t* item) -> void final;
            virtual auto move(Shape_t* item) -> void final;
            virtual auto size() const -> size_t final;

        private:
            size_t size_; /**< @brief Number of shapes currently held in the acceleration structure.*/ 
    };
}}
#endif