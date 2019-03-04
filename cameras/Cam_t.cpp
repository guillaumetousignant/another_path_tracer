#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "Referentials.h"
#include "RandomGenerator_t.h"

#include <iostream> // REMOVE

#define PI 3.141592653589793238463

Cam_t::Cam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image), unif_(0, 1) {
    direction_ = Vec3f(0, 1, 0); // CHECK fill when transformation mats are done
    origin_ = Vec3f(0, 0, 0);
}

Cam_t::~Cam_t() {}

void Cam_t::update() {
    // CHECK fill when transforms are done
}

void Cam_t::raytrace(Scene_t* scene) {
    double tot_subpix;
    double pixel_span_x, pixel_span_y;
    double subpix_span_x, subpix_span_y;
    Vec3f horizontal, vertical;
    
    tot_subpix = subpix_[0]*subpix_[1];
    pixel_span_y = fov_[0]/image_->size_y_;
    pixel_span_x = fov_[1]/image_->size_x_;
    subpix_span_y = pixel_span_y/subpix_[0];
    subpix_span_x = pixel_span_x/subpix_[1];
    horizontal = direction_.cross(up_);
    vertical = horizontal.cross(direction_);

    //std::cout << PI/2.0 + (0.0 - (double)image_->size_y_/2.0 + 0.5)*pixel_span_y << " " << (0.0 - (double)image_->size_x_/2.0 + 0.5)*pixel_span_x << std::endl; // REMOVE

    image_->update();

    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            Vec3f pix_vec = Vec3f(1, PI/2.0 + (j - (double)image_->size_y_/2.0 + 0.5)*pixel_span_y, (i - (double)image_->size_x_/2.0 + 0.5)*pixel_span_x); // Is shit after this line
            //std::cout << pix_vec[0] << " " << pix_vec[1] << " " << pix_vec[2] << std::endl; // REMOVE

            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    
                    double jitter_y = unif_(my_rand::rng);
                    double jitter_x = unif_(my_rand::rng);

                    pix_vec += Vec3f(0, (k - (double)subpix_[0]/2 + jitter_y)*subpix_span_y, (l - (double)subpix_[1]/2 + jitter_x)*subpix_span_x); // Is shit after this line
                    //std::cout << pix_vec[0] << " " << pix_vec[1] << " " << pix_vec[2] << std::endl; // REMOVE
                    pix_vec = to_xyz_offset(pix_vec, direction_, horizontal, vertical); // Is shit after this line

                    //std::cout << pix_vec[0] << " " << pix_vec[1] << " " << pix_vec[2] << std::endl; // REMOVE

                    Ray_t ray = Ray_t(origin_, pix_vec, Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);
                    ray.raycast(scene, max_bounces_, skybox_);
                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void Cam_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name);
}

void Cam_t::show() const {
    // What to do here?
}