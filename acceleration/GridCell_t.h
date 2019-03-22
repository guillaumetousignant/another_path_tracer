#ifndef GRIDCELL_T_H
#define GRIDCELL_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"
#include <list>

class Shape_t;

class GridCell_t : public AccelerationStructure_t{
    public:
        GridCell_t();
        virtual ~GridCell_t();

        std::list<Shape_t*> items_; // Should be list or vector or array?

        virtual void update();
        virtual void intersect(const Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const;
        void add(Shape_t* item);
        void remove(Shape_t* item);
};
#endif