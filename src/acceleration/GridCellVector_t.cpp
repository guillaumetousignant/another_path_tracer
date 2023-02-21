#include "acceleration/GridCellVector_t.hpp"
#include "entities/Shape_t.hpp"
#include <limits>
#include <vector>

using APTracer::Acceleration::GridCellVector_t;

GridCellVector_t::GridCellVector_t() : size_(0) {}

GridCellVector_t::GridCellVector_t(size_t size) : size_(size) {
    items_.reserve(size_);
}

auto GridCellVector_t::intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    double t_temp;
    std::array<double, 2> uv_temp;

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

auto GridCellVector_t::add(Shape_t* item) -> void {
    items_.push_back(item);
}

auto GridCellVector_t::remove(const Shape_t* item) -> void {
    for (size_t i = 0; i < items_.size(); i++) {
        if (items_[i] == item) {
            items_.erase(items_.begin() + i);
            break;
        }
    }
}

auto GridCellVector_t::move(Shape_t* item) -> void {}

auto GridCellVector_t::reserve() -> void {
    items_.reserve(size_);
}

auto GridCellVector_t::operator++() -> GridCellVector_t& {
    ++size_;
    return *this;
}

auto GridCellVector_t::size() const -> size_t {
    return items_.size();
}
