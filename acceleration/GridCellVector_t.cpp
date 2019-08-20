#include "GridCell_t.h"
#include "Shape_t.h"
#include <list>
#include <limits>

#define CELLLIST

#ifdef CELLLIST
GridCell_t::GridCell_t() : items_(std::list<Shape_t*>()) {
#else
GridCell_t::GridCell_t() : items_(nullptr/*std::list<Shape_t*>()*/) {
#endif
    n_obj_ = 0;
}

GridCell_t::~GridCell_t(){
    #ifndef CELLLIST
    if (items_ != nullptr){
        delete [] items_;
    }
    #endif
}

void GridCell_t::update(){
    #ifdef CELLLIST
    for (std::list<Shape_t*>::iterator it = items_.begin(); it != items_.end(); ++it){
        (*it)->update();
    }
    #else
    for (unsigned int i = 0; i < n_obj_; i++){
        items_[i]->update();
    }
    #endif
}

void GridCell_t::intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    double t_temp;
    bool intersected;
    double uv_temp[2];

    hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

    #ifdef CELLLIST
    for (auto it = items_.begin(); it != items_.end(); ++it){
        (*it)->intersection(ray, intersected, t_temp, uv_temp);
        if (intersected && (t_temp < t)){
            hit_obj = *it;
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
    #else
    for (unsigned int i = 0; i < n_obj_; i++){
        items_[i]->intersection(ray, intersected, t_temp, uv_temp);
        if (intersected && (t_temp < t)){
            hit_obj = items_[i];
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
    #endif
}

void GridCell_t::add(Shape_t* item){
    #ifndef CELLLIST
    Shape_t** items2 = new Shape_t*[n_obj_+1];
    for (unsigned int i = 0; i < n_obj_; i++){
        items2[i] = items_[i];
    }
    items2[n_obj_] = item;
    if (items_ != nullptr){
        delete [] items_;
    }
    items_ = items2;
    #else
    items_.push_back(item);
    #endif

    n_obj_++;
}

void GridCell_t::remove(const Shape_t* item){
    #ifdef CELLLIST
    for (std::list<Shape_t*>::iterator it = items_.begin(); it != items_.end(); ++it){
        if ((*it) == item){
            items_.erase(it);
            n_obj_--;
            break;
        }
    }
    #else
    for (unsigned int i = 0; i < n_obj_; i++){
        if (items_[i] == item){
            

            Shape_t** items2 = new Shape_t*[n_obj_-1];
            for (unsigned int j = 0; j < i-1; j++){
                items2[j] = items_[j];
            }
            for (unsigned int j = i+1; j < n_obj_; j++){
                items2[j-1] = items_[j];
            }
            if (items_ != nullptr){
                delete [] items_;
            }
            items_ = items2;

            n_obj_--;
            break;
        }
    }  
    #endif  
}