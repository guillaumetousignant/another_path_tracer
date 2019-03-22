#ifndef ACCELERATIONGRID_T_H
#define ACCELERATIONGRID_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"

class Shape_t;
class GridCell_t;

class AccelerationGrid_t : public AccelerationStructure_t{
    public:
        AccelerationGrid_t(Shape_t** items, unsigned int n_items);
        virtual ~AccelerationGrid_t();

        AccelerationStructure_t** cells_;
        unsigned int n_obj_;
        Vec3f cell_res_;
        Vec3f cell_size_;

        Vec3f coordinates_[2];

        virtual void update();
        virtual void intersect(const Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const;
        virtual void add(Shape_t* item);
        virtual void remove(const Shape_t* item);
        void move(Shape_t* item);
};
#endif