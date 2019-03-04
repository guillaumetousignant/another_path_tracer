#include "DirectionalLight_t.h"
#include "TransformMatrix_t.h"

DirectionalLight_t::DirectionalLight_t(const Vec3f &intensity, TransformMatrix_t* transformation) : intensity_(intensity), transformation_(transformation) {
    radius_ = 0.95; /// CHECK update when transform are done
    direction_ = Vec3f(1.0, 1.0, -1.0).normalize(); // CHECK update when transformations are done
}

DirectionalLight_t::~DirectionalLight_t(){}

void DirectionalLight_t::update(){

}