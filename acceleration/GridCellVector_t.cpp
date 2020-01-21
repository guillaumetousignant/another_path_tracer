#include "GridCellVector_t.h"
#include "Shape_t.h"
#include <vector>
#include <limits>

#define CELLLIST

GridCellVector_t::GridCellVector_t() : items_(std::vector<Shape_t*>()), size_(0) {
    n_obj_ = 0;
}

GridCellVector_t::GridCellVector_t(unsigned int size) : items_(std::vector<Shape_t*>()), size_(size) {
    n_obj_ = 0;
    items_.reserve(size_);
}

GridCellVector_t::~GridCellVector_t(){
}

void GridCellVector_t::update(){
    for (unsigned int i = 0; i < items_.size(); i++){
        items_[i]->update();
    }
}

void GridCellVector_t::intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    double t_temp;
    double uv_temp[2];

    hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

    for (unsigned int i = 0; i < items_.size(); i++){
        if (items_[i]->intersection(ray, t_temp, uv_temp) && (t_temp < t)){
            hit_obj = items_[i];
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
}

void GridCellVector_t::add(Shape_t* item){
    items_.push_back(item);
    n_obj_++;
}

void GridCellVector_t::remove(const Shape_t* item){
    for (unsigned int i = 0; i < items_.size(); i++){
        if (items_[i] == item){
            items_.erase(items_.begin() + i);
            n_obj_--;
            break;
        }
    }
}

void GridCellVector_t::reserve(){
    items_.reserve(size_);
}

GridCellVector_t& GridCellVector_t::operator++(){
    ++size_;
    return *this;
}