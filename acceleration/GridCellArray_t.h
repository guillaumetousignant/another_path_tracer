#ifndef GRIDCELLARRAY_T_H
#define GRIDCELLARRAY_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"

class Shape_t;

class GridCellArray_t final : public AccelerationStructure_t{
    public:
        GridCellArray_t();
        GridCellArray_t(unsigned int size);
        virtual ~GridCellArray_t() final;

        unsigned int size_;
        Shape_t** items_;
        unsigned int increment_size_;

        virtual void update() final;
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const final;
        virtual void add(Shape_t* item) final;
        virtual void remove(const Shape_t* item) final;
        void reserve();
        GridCellArray_t& operator++();
};
#endif