#include "acceleration/GridCellArray_t.h"
#include "entities/Shape_t.h"
#include <limits>

using APTracer::Acceleration::GridCellArray_t;

GridCellArray_t::GridCellArray_t() : size_(0), increment_size_(0) {
    n_obj_ = 0;
}

GridCellArray_t::GridCellArray_t(size_t size) : size_(size), items_(size_), increment_size_(size_) {
    n_obj_ = 0;
}

auto GridCellArray_t::intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* {
    double t_temp;
    std::array<double, 2> uv_temp;

    Shape_t* hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::max();

    for (size_t i = 0; i < n_obj_; i++) {
        if (items_[i]->intersection(ray, t_temp, uv_temp) && (t_temp < t)) {
            hit_obj = items_[i];
            uv = uv_temp;
            t = t_temp;
        }
    }
    return hit_obj;
}

auto GridCellArray_t::add(Shape_t* item) -> void {
    if (n_obj_ < size_) {
        items_[n_obj_] = item;
    }
    else {
        std::vector<Shape_t*> items2(n_obj_+1);
        std::copy(items_.begin(), items_.end(), items2.begin());
        items2[n_obj_] = item;

        items_ = std::move(items2);
        ++size_;
        ++increment_size_;
    }    

    ++n_obj_;
}

auto GridCellArray_t::remove(const Shape_t* item) -> void {
    for (size_t i = 0; i < n_obj_; i++) {
        if (items_[i] == item) {
            for (size_t j = i+1; j < n_obj_; ++j) {
                items_[j-1] = items_[j]; 
            }
            --n_obj_;
            break;
        }
    }  
}

auto GridCellArray_t::move(Shape_t* item) -> void {
    
}

auto GridCellArray_t::reserve() -> void {
    if (n_obj_ > 0) {
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