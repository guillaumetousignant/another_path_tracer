#include "AccelerationGrid_t.h"
#include "Shape_t.h"
#include "GridCell_t.h"

AccelerationGrid_t::AccelerationGrid_t(Shape_t** items, unsigned int n_items) 
    : n_obj_(n_items) {

}

AccelerationGrid_t::~AccelerationGrid_t(){}

void AccelerationGrid_t::update(){

}

void AccelerationGrid_t::intersect(const Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const {

}

void AccelerationGrid_t::add(Shape_t* item){

}

void AccelerationGrid_t::remove(const Shape_t* item){

}

void AccelerationGrid_t::move(Shape_t* item){

}