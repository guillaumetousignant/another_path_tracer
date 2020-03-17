#ifndef ACCELERATIONGRID_T_H
#define ACCELERATIONGRID_T_H

#include "Ray_t.h"
#include "AccelerationStructure_t.h"
#include "Vec3f.h"
#include "Box_t.h"

using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::AccelerationStructure_t;
using APTracer::Shapes::Box_t;

class Shape_t;

namespace APTracer::Acceleration {
    class GridCell_t;
    class AccelerationGrid_t final : public AccelerationStructure_t{
        public:
            AccelerationGrid_t(Shape_t** items, unsigned int n_items, Vec3f* coordinates = nullptr, unsigned int level = 0, unsigned int min_res = 1, unsigned int max_res = 128);
            virtual ~AccelerationGrid_t() final;

            GridCell_t** cells_;
            unsigned int cell_res_[3];
            Vec3f cell_size_;
            Box_t bounding_box_;
            Vec3f coordinates_[2];
            unsigned int level_;
            unsigned int min_res_;
            unsigned int max_res_;

            virtual void update() final;
            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const final;
            virtual void add(Shape_t* item) final;
            virtual void remove(const Shape_t* item) final;
            void move(Shape_t* item);
    };
}
#endif