#ifndef SCENE_T_H
#define SCENE_T_H

//#include "Shape_t.h"
//#include "AccelerationStructure_t.h"
//#include "Mesh_t.h"
#include "Ray_t.h"

class Shape_t;
class AccelerationStructure_t;
class Mesh_t;


class Scene_t{
    public:
        Scene_t();
        ~Scene_t();

        Shape_t** geometry_;
        unsigned int n_obj_;
        AccelerationStructure_t* acc_;

        void add(Shape_t* shape);
        void add(Shape_t** shapes, unsigned int n_shapes);
        void add(Mesh_t* mesh);
        void add(Mesh_t** meshes, unsigned int n_meshes);
        void remove(Shape_t* shape);
        void remove(Shape_t** shapes, unsigned int n_shapes);
        void remove(Mesh_t* mesh);
        void remove(Mesh_t** meshes, unsigned int n_meshes);
        void update();
        void build_acc();
        void intersect_brute(Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const;
        void intersect(Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const;
};

#endif