#ifndef APTRACER_ACCELERATIONGRID_T_HPP
#define APTRACER_ACCELERATIONGRID_T_HPP

#include "acceleration/GridCell_t.hpp"
#include "entities/AccelerationStructure_t.hpp"
#include "entities/Vec3f.hpp"
#include "shapes/Box_t.hpp"
#include <memory>
#include <vector>

namespace APTracer { namespace Acceleration {

    /**
     * @brief The acceleration grid class describes a grid acceleration structure, splitting the space into uniform cells. Cells use lists of shapes, for constant time adding and removal of shapes.
     *
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update, but makes them vulnerable to the "teapot in a stadium" problem, where a single cell can contain a disproportionate amount of geometry.
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span.
     * This structure with its list representation is more geared toward moving scenes, as the cost to remove, add or move shapes from one cell to another is lower.
     */
    class AccelerationGrid_t final : public Entities::AccelerationStructure_t {
        public:
            /**
             * @brief Construct a new top-level AccelerationGrid_t object from an array of shapes.
             *
             * @param items Array of shapes to be added to the acceleration structure.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             */
            AccelerationGrid_t(const std::vector<Entities::Shape_t*>& items, size_t min_res, size_t max_res);

            /**
             * @brief Construct a new AccelerationGrid_t object from an array of shapes.
             *
             * @param items Array of shapes to be added to the acceleration structure.
             * @param coordinates Array of minimum and maximum coordinates covered by the acceleration structure. Set to restrict span, for example to use as a cell within another grid.
             * @param level Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.
             * @param min_res Minimum number of cells for all directions.
             * @param max_res Maximum number of cells for all directions.
             */
            AccelerationGrid_t(const std::vector<Entities::Shape_t*>& items, std::array<Entities::Vec3f, 2> coordinates, unsigned int level, size_t min_res, size_t max_res);

            /**
             * @brief Destroy the AccelerationGrid_t object.
             */
            ~AccelerationGrid_t() final = default;

            /**
             * @brief Copy construct a new AccelerationGrid_t object.
             *
             * @param other Object to copy.
             */
            AccelerationGrid_t(const AccelerationGrid_t& other);

            /**
             * @brief Move construct a new AccelerationGrid_t object.
             *
             * @param other Object to move.
             */
            AccelerationGrid_t(AccelerationGrid_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return AccelerationGrid_t& Reference to this object.
             */
            auto operator=(const AccelerationGrid_t& other) -> AccelerationGrid_t&;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return AccelerationGrid_t& Reference to this object.
             */
            auto operator=(AccelerationGrid_t&& other) noexcept -> AccelerationGrid_t& = default;

            std::vector<std::unique_ptr<GridCell_t>>
                cells_; /**< @brief Array of all the cells contained in the acceleration structure. Cells use lists of shapes, for constant time adding and removal of shapes.*/
            std::array<size_t, 3> cell_res_; /**< @brief Number of cells in the x, y, and z direction.*/
            Entities::Vec3f cell_size_; /**< @brief Span of the cells in the x, y, and z direction.*/
            Shapes::Box_t bounding_box_; /**< @brief Box representing the space encompassed by the grid.*/
            unsigned int level_; /**< @brief Recursion level of the grid. 0 is a top-level grid, 1 is a grid which is a cell of a grid, 2 is a grid within a grid within a grid, etc.*/
            size_t min_res_; /**< @brief Minimum number of cells for all directions.*/
            size_t max_res_; /**< @brief Maximum number of cells for all directions.*/

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
