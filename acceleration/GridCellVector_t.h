#ifndef GRIDCELLVECTOR_T_H
#define GRIDCELLVECTOR_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"
#include <vector>

#define CELLLIST

class Shape_t;

class GridCellVector_t final : public AccelerationStructure_t{
    public:
        GridCellVector_t();
        GridCellVector_t(unsigned int size);
        virtual ~GridCellVector_t() final;

        std::vector<Shape_t*> items_; // Should be list or vector or array?
        unsigned int size_;

        virtual void update() final;
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const final;
        virtual void add(Shape_t* item) final;
        virtual void remove(const Shape_t* item) final;
        void reserve();
        GridCellVector_t& operator++();
};
#endif