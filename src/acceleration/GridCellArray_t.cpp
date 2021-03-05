#include "acceleration/GridCellArray_t.h"
#include "entities/Shape_t.h"
#include <limits>

using APTracer::Acceleration::GridCellArray_t;

GridCellArray_t::GridCellArray_t() : size_(0), items_(nullptr), increment_size_(0) {
    n_obj_ = 0;
}

GridCellArray_t::GridCellArray_t(size_t size) : size_(size), items_(new Shape_t*[size_]), increment_size_(size_) {
    n_obj_ = 0;
}

GridCellArray_t::~GridCellArray_t() {
    delete [] items_;
}

auto GridCellArray_t::intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* {
    double t_temp;
    std::array<double, 2> uv_temp;

    Shape_t* hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

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
        auto items2 = new Shape_t*[n_obj_+1];
        for (size_t i = 0; i < n_obj_; i++) {
            items2[i] = items_[i];
        }
        items2[n_obj_] = item;
        delete [] items_;

        items_ = items2;
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
        auto items_temp = new Shape_t*[increment_size_];
        for (size_t i = 0; i < n_obj_; ++i) {
            items_temp[i] = items_[i];
        }
        delete [] items_;
        items_ = items_temp;
    }
    else {
        delete [] items_;
        items_ = new Shape_t*[increment_size_];        
    }
    size_ = increment_size_;
}

auto GridCellArray_t::operator++() -> GridCellArray_t& {
    ++increment_size_;
    return *this;
}