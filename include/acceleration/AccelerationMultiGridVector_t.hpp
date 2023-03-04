#ifndef APTRACER_ACCELERATIONMULTIGRIDVECTOR_T_HPP
#define APTRACER_ACCELERATIONMULTIGRIDVECTOR_T_HPP

#include "entities/AccelerationStructure_t.hpp"
#include "entities/Vec3f.hpp"
#include "shapes/Box_t.hpp"
#include <memory>
#include <vector>

namespace APTracer { namespace Acceleration {
    /**
     * @brief The acceleration multi grid vector class describes a multi-level grid acceleration structure, splitting the space into uniform cells or grids, depending on the number of shapes in a
     * cell. Cells use vectors of shapes, for fast iterating and low memory use.
     *
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update. Multi level grids enable cells containing several shapes to be changed to grids. This can help alleviate the "Teapot in a stadium problem".
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span. Input parameters can set the number of shapes before a subgrid is created, and the
     * maximum grid recursion level. This structure with its vector representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is higher.
     * With vectors, iteration on shapes is slightly faster, and the memory used is low.
     */
    class AccelerationMultiGridVector_t final : public Entities::AccelerationStructure_t {
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
            AccelerationMultiGridVector_t(const std::vector<Entities::Shape_t*>& items, size_t min_res, size_t max_res, size_t max_cell_content, unsigned int max_grid_level);

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
            AccelerationMultiGridVector_t(const std::vector<Entities::Shape_t*>& items,
                                          std::array<Entities::Vec3f, 2> coordinates,
                                          unsigned int level,
                                          size_t min_res,
                                          size_t max_res,
                                          size_t max_cell_content,
                                          unsigned int max_grid_level);

            /**
             * @brief Destroy the AccelerationMultiGridVector_t object.
             */
            ~AccelerationMultiGridVector_t() final = default;

            /**
             * @brief Copy construct a new AccelerationMultiGridVector_t object.
             *
             * @param other Object to copy.
             */
            AccelerationMultiGridVector_t(const AccelerationMultiGridVector_t& other);

            /**
             * @brief Move construct a new AccelerationMultiGridVector_t object.
             *
             * @param other Object to move.
             */
            AccelerationMultiGridVector_t(AccelerationMultiGridVector_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return AccelerationMultiGridVector_t& Reference to this object.
             */
            auto operator=(const AccelerationMultiGridVector_t& other) -> AccelerationMultiGridVector_t&;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return AccelerationMultiGridVector_t& Reference to this object.
             */
            auto operator=(AccelerationMultiGridVector_t&& other) noexcept -> AccelerationMultiGridVector_t& = default;

            std::vector<std::unique_ptr<AccelerationStructure_t>>
                cells_; /**< @brief Array of all the cells and grids contained in the acceleration structure. Cells use vectors of shapes, for fast iterating and low memory use.*/
            std::array<size_t, 3> cell_res_; /**< @brief Number of cells in the x, y, and z direction.*/
            Entities::Vec3f cell_size_; /**< @brief Span of the cells in the x, y, and z direction.*/
            Shapes::Box_t bounding_box_; /**< @brief Box representing the space encompassed by the grid.*/
            unsigned int level_; /**< @brief Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.*/
            size_t min_res_; /**< @brief Minimum number of cells for all directions.*/
            size_t max_res_; /**< @brief Maximum number of cells for all directions.*/
            size_t max_cell_content_; /**< @brief Maximum number of shapes in a cell before it is changed to a subgrid.*/
            unsigned int max_grid_level_; /**< @brief Maximum recursion level of the grid. 0 is no subgrids, 1 means cells from the top-level grid can be single level grids, etc.*/

            auto intersect(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Entities::Shape_t* final;
            auto add(Entities::Shape_t* item) -> void final;
            auto remove(Entities::Shape_t* item) -> void final;
            auto move(Entities::Shape_t* item) -> void final;
            auto size() const -> size_t final;
            auto clone() const -> std::unique_ptr<AccelerationStructure_t> final;

        private:
            size_t size_; /**< @brief Number of shapes currently held in the acceleration structure.*/
    };
}}
#endif
