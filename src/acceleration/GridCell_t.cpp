#include "acceleration/GridCell_t.h"
#include "entities/Shape_t.h"
#include <list>
#include <limits>

#define CELLLIST

using APTracer::Acceleration::GridCell_t;

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

Shape_t* GridCell_t::intersect(const Ray_t &ray, double &t, double (&uv)[2]) const {
    double t_temp;
    double uv_temp[2];

    Shape_t* hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

    for (auto it = items_.begin(); it != items_.end(); ++it){
        if ((*it)->intersection(ray, t_temp, uv_temp) && (t_temp < t)){
            hit_obj = *it;
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
    return hit_obj;
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