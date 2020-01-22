#ifndef SCENE_T_H
#define SCENE_T_H

#include "Ray_t.h"

class Shape_t;
class AccelerationStructure_t;
class MeshTop_t;


class Scene_t{
    public:
        Scene_t();
        ~Scene_t();

        Shape_t** geometry_;
        unsigned int n_obj_;
        AccelerationStructure_t* acc_;

        void add(Shape_t* shape);
        void add(Shape_t** shapes, unsigned int n_shapes);
        void add(MeshTop_t* mesh);
        void add(MeshTop_t** meshes, unsigned int n_meshes);
        void remove(Shape_t* shape);
        void remove(Shape_t** shapes, unsigned int n_shapes);
        void remove(MeshTop_t* mesh);
        void remove(MeshTop_t** meshes, unsigned int n_meshes);
        void update();
        void build_acc();
        Shape_t* intersect_brute(const Ray_t &ray, double &t, double (&uv)[2]) const;
        Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const;
};

#endif