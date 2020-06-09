#ifndef ACCELERATIONGRIDARRAY_T_H
#define ACCELERATIONGRIDARRAY_T_H

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
    class GridCellArray_t;

    /**
     * @brief The acceleration grid array class describes a grid acceleration structure, splitting the space into uniform cells. Cells use arrays of shapes, for fast iteraring and lowest memory use.
     * 
     * Grid acceleration structures split the space they span into uniform cells, according to a 3D resolution. Rays are intersected with the cells before being intersected with the shapes they
     * contain. This makes them fast to build and update, but makes them vulnerable to the "teapot in a stadium" problem, where a single cell can contain a disproportionate amount of geometry.
     * A heuristic is used to determine grid resolution, taking into account the number of shapes and the span.
     * This structure with its array representation is more geared toward static scenes, as the cost to remove, add or move shapes from one cell to another is higher. With arrays, iteration on
     * shapes is slightly lower, and the memory used is lowest.
     */
    class AccelerationGridArray_t final : public AccelerationStructure_t{
        public:
            AccelerationGridArray_t(Shape_t** items, unsigned int n_items, const Vec3f* coordinates = nullptr, unsigned int level = 0, unsigned int min_res = 1, unsigned int max_res = 128);
            virtual ~AccelerationGridArray_t() final;

            GridCellArray_t** cells_; /**< @brief Array of all the cells contained in the acceleration structure.*/
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