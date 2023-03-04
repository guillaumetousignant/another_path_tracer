#ifndef APTRACER_GRIDCELLVECTOR_T_HPP
#define APTRACER_GRIDCELLVECTOR_T_HPP

#include "entities/AccelerationStructure_t.hpp"
#include <vector>

namespace APTracer { namespace Acceleration {
    /**
     * @brief The grid cell vector class describes a cell from a grid, the fundamental object containing shapes. Shapes are stored in a vector, for fast iterating and low memory use.
     *
     * This structure with its vector representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is higher. With vectors, iteration on
     * shapes is slightly faster, and the memory used is low.
     */
    class GridCellVector_t final : public Entities::AccelerationStructure_t {
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
            explicit GridCellVector_t(size_t size);

            std::vector<Entities::Shape_t*> items_; /**< @brief Vector of shapes contained in the cell. This allows fast iterating and low memory use.*/
            size_t size_; /**< @brief Size that will be allocated next time 'reserve()' is called. Increased with 'operator++'.*/

            auto intersect(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Entities::Shape_t* final;
            auto add(Entities::Shape_t* item) -> void final;
            auto remove(Entities::Shape_t* item) -> void final;
            auto move(Entities::Shape_t* item) -> void final;
            auto size() const -> size_t final;
            auto clone() const -> std::unique_ptr<AccelerationStructure_t> final;

            /**
             * @brief Reserves 'size_' in the shape vector.
             */
            auto reserve() -> void;

            /**
             * @brief Increase 'size_', to eventually reserve the new size of the vector with 'reserve()'.
             *
             * @return GridCellVector_t& Reference to this, for chaining operations.
             */
            auto operator++() -> GridCellVector_t&;
    };
}}
#endif
