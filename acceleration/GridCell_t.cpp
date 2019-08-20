#include "GridCell_t.h"
#include "Shape_t.h"
#include <list>
#include <limits>

#define CELLLIST

GridCell_t::GridCell_t() : items_(std::list<Shape_t*>()) {
    n_obj_ = 0;
}

GridCell_t::~GridCell_t(){
}

void GridCell_t::update(){
    for (std::list<Shape_t*>::iterator it = items_.begin(); it != items_.end(); ++it){
        (*it)->update();
    }
}

void GridCell_t::intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    double t_temp;
    bool intersected;
    double uv_temp[2];

    hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

    for (auto it = items_.begin(); it != items_.end(); ++it){
        (*it)->intersection(ray, intersected, t_temp, uv_temp);
        if (intersected && (t_temp < t)){
            hit_obj = *it;
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
}

void GridCell_t::add(Shape_t* item){
    items_.push_back(item);
    n_obj_++;
}

void GridCell_t::remove(const Shape_t* item){
    for (std::list<Shape_t*>::iterator it = items_.begin(); it != items_.end(); ++it){
        if ((*it) == item){
            items_.erase(it);
            n_obj_--;
            break;
        }
    }
}