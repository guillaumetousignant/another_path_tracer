#ifndef GRIDCELL_T_H
#define GRIDCELL_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"
#include <list>

#define CELLLIST

class Shape_t;

class GridCell_t : public AccelerationStructure_t{
    public:
        GridCell_t();
        virtual ~GridCell_t();

        #ifdef CELLLIST
        std::list<Shape_t*> items_; // Should be list or vector or array?
        #else
        Shape_t** items_;
        #endif

        virtual void update();
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const;
        virtual void add(Shape_t* item);
        virtual void remove(const Shape_t* item);
};
#endif