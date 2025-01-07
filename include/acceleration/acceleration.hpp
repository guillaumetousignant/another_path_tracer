#ifndef APTRACER_ACCELERATION_HPP
#define APTRACER_ACCELERATION_HPP

namespace APTracer {
    /**
     * @brief Contains the different acceleration structure types that can be used.
     *
     * Acceleration structures sort shapes in space, and can be intersected to find the closest intersection of a ray.
     * This namespace contains both top-level acceleration structures and the sub-classes they need.
     */
    namespace Acceleration {
    }
}

#include "AccelerationGrid_t.hpp"
#include "AccelerationGridArray_t.hpp"
#include "AccelerationGridVector_t.hpp"
#include "AccelerationMultiGrid_t.hpp"
#include "AccelerationMultiGridArray_t.hpp"
#include "GridCell_t.hpp"
#include "GridCellArray_t.hpp"
#include "GridCellVector_t.hpp"

#endif
