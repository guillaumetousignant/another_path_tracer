#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "Referentials.h"
#include "RandomGenerator_t.h"

#define PI 3.141592653589793238463

Cam_t::Cam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind, RandomGenerator_t* rng) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image), rng_(rng) {
    direction_ = Vec3f(0, 1, 0); // CHECK fill when transformation mats are done
    origin_ = Vec3f(0, 0, 0);
}

Cam_t::~Cam_t() {}

void Cam_t::update() {
    // CHECK fill when transforms are done
}

void Cam_t::raytrace(Scene_t* scene) const {
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

    image_->update();

    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            Vec3f pix_vec = Vec3f(1, PI/2 + (j - image_->size_y_/2 + 0.5)*pixel_span_y, (i - image_->size_x_/2 + 0.5)*pixel_span_x); // Or declare above?
        
            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    
                    double jitter_y = rng_->rand();
                    double jitter_x = 0; //unif_(rng); // Or declare above?

                    pix_vec += Vec3f(0, (k - subpix_[0]/2 + jitter_y)*subpix_span_y, (l - subpix_[1]/2 + jitter_x)*subpix_span_x);
                    pix_vec = to_xyz_offset(pix_vec, direction_, horizontal, vertical);
                
                    Ray_t ray = Ray_t(origin_, pix_vec, Vec3f(), Vec3f(1, 1, 1), medium_list_);
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
    // Call imgbuffer write
}

void Cam_t::show() const {
    // What to do here?
}