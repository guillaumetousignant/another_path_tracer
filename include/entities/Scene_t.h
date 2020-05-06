#ifndef SCENE_T_H
#define SCENE_T_H

#include <vector>
#include "entities/Ray_t.h"

namespace APTracer { namespace Shapes {
    class MeshTop_t;
}}

using APTracer::Shapes::MeshTop_t;

namespace APTracer { namespace Entities {
    class Shape_t;
    class AccelerationStructure_t;

    /**
     * @brief The scene class holds the shapes representing a scene to be rendered, and creates an acceleration structure containing them.
     *  
     * Holds an array of shapes to be intersected by rays. Those shapes are sorted in an acceleration structure owned by the scene for 
     * faster intersection. The acceleration has to be built using build_acc before intersecting the scene. It can also be intersected
     * without the acceleration structure with intersect_brute, at a much slower pace when there are many shapes. Shapes can be added and 
     * removed from the scene, but these operations are costly so should be batched.
     */
    class Scene_t{
        public:
            Scene_t();
            Scene_t(Shape_t* shape);
            Scene_t(Shape_t** shapes, unsigned int n_shapes);
            Scene_t(MeshTop_t* mesh);
            Scene_t(MeshTop_t** meshes, unsigned int n_meshes);
            ~Scene_t();

            std::vector<Shape_t*> geometry_;
            AccelerationStructure_t* acc_;

            void add(Shape_t* shape);
            void add(Shape_t** shapes, unsigned int n_shapes);
            void add(MeshTop_t* mesh);
            void add(MeshTop_t** meshes, unsigned int n_meshes);
            void remove(Shape_t* shape);
            void remove(Shape_t** shapes, unsigned int n_shapes);

            /**
             * @brief Removes a mesh from the scene.
             * 
             * This works by finding the first shape pointer of the mesh and deleting it and the n_tris next shapes.
             * This works fine if the mesh is added with add and if the triangles of the mesh are not added individually
             * out of order.
             * 
             * @param mesh Mesh to be removed from the scene.
             */
            void remove(MeshTop_t* mesh);

            /**
             * @brief Removes multiple meshes from the scene.
             * 
             * This works by finding the first shape pointer of the meshes and deleting it and the n_tris next shapes.
             * This works fine if the meshes are added with add and if the triangles of the meshes are not added individually
             * out of order.
             * 
             * @param meshes Array of meshes to be removed from the scene.
             * @param n_meshes Number of meshes to be removed from the scene.
             */
            void remove(MeshTop_t** meshes, unsigned int n_meshes);
            void update();
            void build_acc();
            Shape_t* intersect_brute(const Ray_t &ray, double &t, double (&uv)[2]) const;
            Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const;
    };
}}

#endif