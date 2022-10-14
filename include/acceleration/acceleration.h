#ifndef APTRACER_ACCELERATION_H
#define APTRACER_ACCELERATION_H

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

#include "AccelerationGrid_t.h"
#include "AccelerationGridArray_t.h"
#include "AccelerationGridVector_t.h"
#include "AccelerationMultiGrid_t.h"
#include "AccelerationMultiGridArray_t.h"
#include "GridCell_t.h"
#include "GridCellArray_t.h"
#include "GridCellVector_t.h"

#endif