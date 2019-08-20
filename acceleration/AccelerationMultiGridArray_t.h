#ifndef ACCELERATIONMULTIGRIDARRAY_T_H
#define ACCELERATIONMULTIGRIDARRAY_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"

class Shape_t;
class Box_t;

class AccelerationMultiGridArray_t final : public AccelerationStructure_t{
    public:
        AccelerationMultiGridArray_t(Shape_t** items, unsigned int n_items, Vec3f* coordinates = nullptr, unsigned int level = 0, unsigned int min_res = 1, unsigned int max_res = 128, unsigned int max_cell_content = 32, unsigned int max_grid_level = 1);
        virtual ~AccelerationMultiGridArray_t() final;

        AccelerationStructure_t** cells_;
        unsigned int cell_res_[3];
        Vec3f cell_size_;
        Box_t* bounding_box_;
        Vec3f coordinates_[2];
        unsigned int level_;
        unsigned int min_res_;
        unsigned int max_res_;
        unsigned int max_cell_content_;
        unsigned int max_grid_level_;

        virtual void update() final;
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const final;
        virtual void add(Shape_t* item) final;
        virtual void remove(const Shape_t* item) final;
        void move(Shape_t* item);
};
#endif