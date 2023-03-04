#ifndef APTRACER_GRIDCELL_T_HPP
#define APTRACER_GRIDCELL_T_HPP

#include "entities/AccelerationStructure_t.hpp"
#include <list>

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in a list, for constant time adding and removal of shapes.
     *
     * This structure with its list representation is more geared toward moving scenes, as the cost to remove, add or move shapes from one cell to another is lower.
     */
    class GridCell_t final : public Entities::AccelerationStructure_t {
        public:
            /**
             * @brief Construct a new GridCell_t object containing no shapes.
             */
            GridCell_t() = default;

            std::list<Entities::Shape_t*> items_; /**< @brief List of shapes contained in the cell. This allows constant time adding and removal of shapes.*/

            auto intersect(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Entities::Shape_t* final;
            auto add(Entities::Shape_t* item) -> void final;
            auto remove(Entities::Shape_t* item) -> void final;
            auto move(Entities::Shape_t* item) -> void final;
            auto size() const -> size_t final;
            auto clone() const -> std::unique_ptr<AccelerationStructure_t> final;
    };
}}
#endif
