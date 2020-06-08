#ifndef SHAPES_H
#define SHAPES_H

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
    namespace Shapes {}
}

#include "Box_t.h"
#include "Mesh_t.h"
#include "MeshMotionblur_t.h"
#include "MeshTop_t.h"
#include "Sphere_t.h"
#include "SphereMotionblur_t.h"
#include "Triangle_t.h"
#include "TriangleMesh_t.h"
#include "TriangleMeshMotionblur_t.h"
#include "TriangleMotionblur_t.h"

#endif