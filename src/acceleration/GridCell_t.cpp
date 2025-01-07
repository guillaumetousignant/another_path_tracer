#include "acceleration/GridCell_t.hpp"
#include <limits>
#include <list>

using APTracer::Acceleration::GridCell_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

auto GridCell_t::intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    double t_temp = std::numeric_limits<double>::max();
    std::array<double, 2> uv_temp{};

    Shape_t* hit_obj = nullptr; // dunno if this is needed
    t                = std::numeric_limits<double>::max();

    for (auto* shape: items_) {
        if (shape->intersection(ray, t_temp, uv_temp) && (t_temp < t)) {
            hit_obj = shape;
            uv      = uv_temp;
            t       = t_temp;
        }
    }
    return hit_obj;
}

auto GridCell_t::add(Shape_t* item) -> void {
    items_.push_back(item);
}

auto GridCell_t::remove(Shape_t* const item) -> void {
    items_.remove(item);
}

auto GridCell_t::move(Shape_t* item) -> void {}

auto GridCell_t::size() const -> size_t {
    return items_.size();
}

auto GridCell_t::clone() const -> std::unique_ptr<AccelerationStructure_t> {
    return std::make_unique<GridCell_t>(*this);
}
