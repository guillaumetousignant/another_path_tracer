#ifndef ACCELERATIONMULTIGRID_T_H
#define ACCELERATIONMULTIGRID_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"

class Shape_t;
class GridCell_t;
class Box_t;

class AccelerationMultiGrid_t final : public AccelerationStructure_t{
    public:
        AccelerationMultiGrid_t(Shape_t** items, unsigned int n_items, Vec3f* coordinates = nullptr, unsigned int level = 0);
        virtual ~AccelerationMultiGrid_t() final;

        AccelerationStructure_t** cells_;
        unsigned int cell_res_[3];
        Vec3f cell_size_;
        Box_t* bounding_box_;
        Vec3f coordinates_[2];
        unsigned int level_;

        virtual void update() final;
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const final;
        virtual void add(Shape_t* item) final;
        virtual void remove(const Shape_t* item) final;
        void move(Shape_t* item);
};
#endif