#ifndef ACCELERATIONGRIDVECTOR_T_H
#define ACCELERATIONGRIDVECTOR_T_H

#include "entities/Ray_t.h"
#include "entities/AccelerationStructure_t.h"
#include "entities/Vec3f.h"
#include "shapes/Box_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;
using APTracer::Shapes::Box_t;

namespace APTracer { namespace Acceleration {
    class GridCellVector_t;

    class AccelerationGridVector_t final : public AccelerationStructure_t{
        public:
            AccelerationGridVector_t(Shape_t** items, unsigned int n_items, Vec3f* coordinates = nullptr, unsigned int level = 0, unsigned int min_res = 1, unsigned int max_res = 128);
            virtual ~AccelerationGridVector_t() final;

            GridCellVector_t** cells_;
            unsigned int cell_res_[3];
            Vec3f cell_size_;
            Box_t bounding_box_;
            unsigned int level_;
            unsigned int min_res_;
            unsigned int max_res_;

            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            virtual void move(Shape_t* item) final;
    };
}}
#endif