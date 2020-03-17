#ifndef GRIDCELL_T_H
#define GRIDCELL_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"
#include <list>

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;

class Shape_t;

namespace APTracer::Acceleration {
    class GridCell_t final : public AccelerationStructure_t{
        public:
            GridCell_t();
            virtual ~GridCell_t() final;

            std::list<Shape_t*> items_; // Should be list or vector or array?

            virtual void update() final;
            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
    };
}
#endif