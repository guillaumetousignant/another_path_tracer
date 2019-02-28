#include "Camera_t.h"
#include "TransformMatrix_t.h"
#include <iostream>

Camera_t::Camera_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], std::list <Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
            : transformation_(transformation), filename_(filename), fov_{fov[0], fov[1]}, subpix_{subpix[0], subpix[1]}, medium_list_(medium_list), 
            skybox_(skybox), max_bounces_(max_bounces), gammaind_(gammaind), up_(up), up_buffer_(up) {
    origin_ = transformation->multVec(Vec3f(0, 0, 0));
    TransformMatrix_t transform_norm = transformation->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0, 1, 0));  
}

void Camera_t::accumulate(Scene_t* scene, unsigned int n_iter /*= 1000000000*/) const {
    unsigned int n = 0;
    while (n < n_iter){
        n++;
        raytrace(scene);
        std::cout << "Iteration " << n << " done." << std::endl;

        show();
        write();
    }
}

void Camera_t::setUp(Vec3f &new_up){
    up_buffer_ = new_up;
}