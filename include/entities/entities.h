#ifndef APTRACER_ENTITIES_ENTITIES_H
#define APTRACER_ENTITIES_ENTITIES_H
namespace APTracer { 
    /**
     * @brief Contains the fundamental types of the project. 
     * 
     * Most of the entities represent interface classes, to be specialized in their respective namespaces.
     * Some entities, such as Vec3f, represent basic types used throughout the program.
     */
    namespace Entities {}
}

#include "AccelerationStructure_t.h"
#include "Camera_t.h"
#include "DirectionalLight_t.h"
#include "ImgBuffer_t.h"
#include "ImgBufferOpenGL_t.h"
#include "Material_t.h"
#include "MaterialMap_t.h"
#include "Medium_t.h"
#include "MeshGeometry_t.h"
#include "OpenGLRenderer_t.h"
#include "RandomGenerator_t.h"
#include "Ray_t.h"
#include "Scene_t.h"
#include "Shape_t.h"
#include "Skybox_t.h"
#include "Texture_t.h"
#include "TransformMatrix_t.h"
#include "Vec3f.h"

#endif