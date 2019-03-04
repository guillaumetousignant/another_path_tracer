#include "TransformMatrix_t.h"

TransformMatrix_t::TransformMatrix_t(){

}

TransformMatrix_t::~TransformMatrix_t(){

}

void TransformMatrix_t::rotateXAxis(double angle){

}

void TransformMatrix_t::rotateYAxis(double angle){

}

void TransformMatrix_t::rotateZAxis(double angle){

}

void TransformMatrix_t::rotateX(double angle){

}

void TransformMatrix_t::rotateY(double angle){

}

void TransformMatrix_t::rotateZ(double angle){

}

void TransformMatrix_t::rotateAxis(const Vec3f &vec, double angle){

}

void TransformMatrix_t::rotate(const Vec3f &vec, double angle){

}

void TransformMatrix_t::translate(const Vec3f &vec){

}

void TransformMatrix_t::scaleAxis(const Vec3f &vec){

}

void TransformMatrix_t::scaleAxis(double fac){

}

void TransformMatrix_t::scale(const Vec3f &vec){

}

void TransformMatrix_t::scale(double fac){

}

void TransformMatrix_t::reflect(const Vec3f &vec){

}

void TransformMatrix_t::shear(const Vec3f &vec){

}

void TransformMatrix_t::transpose(){

}

void TransformMatrix_t::invert(){

}

void TransformMatrix_t::neg(){

}

Vec3f TransformMatrix_t::multVec(const Vec3f &vec){
    return Vec3f();
}

Vec3f TransformMatrix_t::multDir(const Vec3f &vec){
    return Vec3f();
}

TransformMatrix_t TransformMatrix_t::transformDir(){
    return TransformMatrix_t();
}