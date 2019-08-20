#include "GridCellArray_t.h"
#include "Shape_t.h"
#include <limits>

GridCellArray_t::GridCellArray_t() : size_(0), items_(nullptr) {
    n_obj_ = 0;
}

GridCellArray_t::GridCellArray_t(unsigned int size) : size_(size), items_(new Shape_t*[size_]) {
    n_obj_ = 0;
}

GridCellArray_t::~GridCellArray_t(){
    if (items_ != nullptr){
        delete [] items_;
    }
}

void GridCellArray_t::update(){
    for (unsigned int i = 0; i < n_obj_; i++){
        items_[i]->update();
    }
}

void GridCellArray_t::intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    double t_temp;
    bool intersected;
    double uv_temp[2];

    hit_obj = nullptr; // dunno if this is needed    
    t = std::numeric_limits<double>::infinity();

    for (unsigned int i = 0; i < n_obj_; i++){
        items_[i]->intersection(ray, intersected, t_temp, uv_temp);
        if (intersected && (t_temp < t)){
            hit_obj = items_[i];
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
}

void GridCellArray_t::add(Shape_t* item){
    if (n_obj_ < size_){
        items_[n_obj_] = item;
    }
    else {
        Shape_t** items2 = new Shape_t*[n_obj_+1];
        for (unsigned int i = 0; i < n_obj_; i++){
            items2[i] = items_[i];
        }
        items2[n_obj_] = item;
        if (items_ != nullptr){
            delete [] items_;
        }
        items_ = items2;
        ++size_;
    }    

    ++n_obj_;
}

void GridCellArray_t::remove(const Shape_t* item){
    for (unsigned int i = 0; i < n_obj_; i++){
        if (items_[i] == item){
            for (unsigned int j = i+1; j < n_obj_; ++j){
                items_[j-1] = items_[j]; 
            }
            --n_obj_;
            break;
        }
    }  
}