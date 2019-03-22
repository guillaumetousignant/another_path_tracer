#include "Mesh_t.h"
#include "MeshGeometry_t.h"
#include "MaterialMap_t.h"
#include "TriangleMesh_t.h"
#include <iostream>
#include <limits>

Mesh_t::Mesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom) 
    : Shape_t(material, transform_matrix), geom_(geom), n_tris_(geom->n_tris_) {
    
    triangles_ = new Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        this->createTriangle(material, transformation_, geom_, i);
    }
}

Mesh_t::Mesh_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom) 
    : Shape_t(materialmap->getFirst(), transform_matrix), geom_(geom), n_tris_(geom->n_tris_) {

    triangles_ = new Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i] = this->createTriangle(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}        

Mesh_t::~Mesh_t(){
    if (triangles_ != nullptr){
        for (unsigned int i = 0; i < n_tris_; i++){
            if (triangles_[i] != nullptr){
                delete triangles_[i];
            }
        }
        delete [] triangles_;
    }
}

void Mesh_t::update(){
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i]->update();
    }
}

void Mesh_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
    t = std::numeric_limits<double>::infinity();
    intersected = false;
    uv[0] = 0;
    uv[1] = 0;
    std::cout << "Warning, do not intersect meshes directly." << std::endl;   
}

void Mesh_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    tuv[0] = 0;
    tuv[1] = 0;
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

void Mesh_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

Vec3f Mesh_t::mincoord() const {
    Vec3f coord = Vec3f(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    for (unsigned int i = 0; i < n_tris_; i++){
        coord.min(triangles_[i]->mincoord());
    }    
    return coord;
}

Vec3f Mesh_t::maxcoord() const {
    Vec3f coord = Vec3f(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    for (unsigned int i = 0; i < n_tris_; i++){
        coord.max(triangles_[i]->maxcoord());
    }
    return coord;
}

Shape_t* Mesh_t::createTriangle(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index){
    return new TriangleMesh_t(material, transform_matrix, geom, index);
}