#include "acceleration/GridCellArray_t.hpp"
#include <algorithm>
#include <limits>

using APTracer::Acceleration::GridCellArray_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

GridCellArray_t::GridCellArray_t() : size_(0), increment_size_(0) {}

GridCellArray_t::GridCellArray_t(size_t size) : size_(size), items_(size_), increment_size_(size_) {}

auto GridCellArray_t::intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    double t_temp = std::numeric_limits<double>::max();
    std::array<double, 2> uv_temp{};

    Shape_t* hit_obj = nullptr; // dunno if this is needed
    t                = std::numeric_limits<double>::max();

    for (auto* item: items_) {
        if (item->intersection(ray, t_temp, uv_temp) && (t_temp < t)) {
            hit_obj = item;
            uv      = uv_temp;
            t       = t_temp;
        }
    }
    return hit_obj;
}

auto GridCellArray_t::add(Shape_t* item) -> void {
    if (items_.size() < size_) {
        items_[items_.size()] = item;
    }
    else {
        std::vector<Shape_t*> items2(items_.size() + 1);
        std::copy(items_.begin(), items_.end(), items2.begin());
        items2[items_.size()] = item;

        items_ = std::move(items2);
        ++size_;
        ++increment_size_;
    }
}

auto GridCellArray_t::remove(Shape_t* const item) -> void {
    items_.erase(std::remove(items_.begin(), items_.end(), item), items_.end());
}

auto GridCellArray_t::move(Shape_t* item) -> void {}

auto GridCellArray_t::reserve() -> void {
    if (!items_.empty()) {
        std::vector<Shape_t*> items_temp(increment_size_);
        std::copy(items_.begin(), items_.end(), items_temp.begin());
        items_ = std::move(items_temp);
    }
    else {
        items_ = std::vector<Shape_t*>(increment_size_);
    }
    size_ = increment_size_;
}

auto GridCellArray_t::operator++() -> GridCellArray_t& {
    ++increment_size_;
    return *this;
}

auto GridCellArray_t::size() const -> size_t {
    return items_.size();
}

auto GridCellArray_t::clone() const -> std::unique_ptr<AccelerationStructure_t> {
    return std::make_unique<GridCellArray_t>(*this);
}
