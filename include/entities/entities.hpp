#ifndef APTRACER_ENTITIES_ENTITIES_HPP
#define APTRACER_ENTITIES_ENTITIES_HPP
namespace APTracer {
    /**
     * @brief Contains the fundamental types of the project.
     *
     * Most of the entities represent interface classes, to be specialized in their respective namespaces.
     * Some entities, such as Vec3f, represent basic types used throughout the program.
     */
    namespace Entities {
    }
}

#include "AccelerationStructure_t.hpp"
#include "Camera_t.hpp"
#include "DirectionalLight_t.hpp"
#include "ImgBuffer_t.hpp"
#include "ImgBufferOpenGL_t.hpp"
#include "Material_t.hpp"
#include "MaterialMap_t.hpp"
#include "Medium_t.hpp"
#include "MeshGeometry_t.hpp"
#include "OpenGLRenderer_t.hpp"
#include "RandomGenerator_t.hpp"
#include "Ray_t.hpp"
#include "Scene_t.hpp"
#include "Shape_t.hpp"
#include "Skybox_t.hpp"
#include "Texture_t.hpp"
#include "TransformMatrix_t.hpp"
#include "Vec3f.hpp"

#endif