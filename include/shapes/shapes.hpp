#ifndef APTRACER_SHAPES_SHAPES_HPP
#define APTRACER_SHAPES_SHAPES_HPP

namespace APTracer {
    /**
     * @brief Contains different shape types that can be used.
     *
     * Shapes can be intersected with rays, and can provide geometric information
     * about these intersections. Some shapes support motion blur, and retain information
     * about their past state. Meshes are also contained here. Meshes represent collection
     * of shapes with the same transformation matrix, possibly taking their geometry from
     * the same source.
     */
    namespace Shapes {
    }
}

#include "Box_t.hpp"
#include "Mesh_t.hpp"
#include "MeshMotionblur_t.hpp"
#include "MeshTop_t.hpp"
#include "Sphere_t.hpp"
#include "SphereMotionblur_t.hpp"
#include "Triangle_t.hpp"
#include "TriangleMesh_t.hpp"
#include "TriangleMeshMotionblur_t.hpp"
#include "TriangleMotionblur_t.hpp"

#endif
