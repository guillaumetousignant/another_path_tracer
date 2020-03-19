#ifndef GRIDCELLARRAY_T_H
#define GRIDCELLARRAY_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"

namespace APTracer::Entities {
    class Shape_t;
}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;

namespace APTracer::Acceleration {
    class GridCellArray_t final : public AccelerationStructure_t{
        public:
            GridCellArray_t();
            GridCellArray_t(unsigned int size);
            virtual ~GridCellArray_t() final;

            unsigned int size_;
            Shape_t** items_;
            unsigned int increment_size_;

            virtual void update() final;
            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            void reserve();
            GridCellArray_t& operator++();
    };
}
#endif