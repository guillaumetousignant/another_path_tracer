#include "RecCamAperture_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "Referentials.h"
#include "RandomGenerator_t.h"
#include <cmath>
#include <limits>

#define PI 3.141592653589793238463

RecCamAperture_t::RecCamAperture_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double gammaind) 
    : RecCam_t(transformation, filename, up, fov, subpix, image, medium_list, skybox, max_bounces, gammaind),  
    focal_length_(focal_length), aperture_(aperture), focal_length_buffer_(focal_length) {}

RecCamAperture_t::~RecCamAperture_t() {}

void RecCamAperture_t::update() {
    origin_ = transformation_->multVec(Vec3f(0.0, 0.0, 0.0));
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;
}

void RecCamAperture_t::raytrace(const Scene_t* scene) {
    double tot_subpix;
    Vec3f pixel_span_x, pixel_span_y;
    Vec3f subpix_span_x, subpix_span_y;
    Vec3f horizontal, vertical;
    Vec3f focus_point;
    
    horizontal = direction_.cross(up_).normalize();
    vertical = horizontal.cross(direction_).normalize();
    focus_point = origin_ + direction_ * focal_length_;
    tot_subpix = subpix_[0]*subpix_[1];
    pixel_span_y = vertical * focal_length_ * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    pixel_span_x = horizontal * focal_length_ * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    subpix_span_y = pixel_span_y/subpix_[0];
    subpix_span_x = pixel_span_x/subpix_[1];

    image_->update();

    #pragma omp parallel for collapse(2) schedule(guided)
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            Vec3f pix_vec = focus_point - pixel_span_y * ((double)j - (double)image_->size_y_/2.0 + 0.5) + pixel_span_x * ((double)i - (double)image_->size_x_/2.0 + 0.5);
            
            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    double rand_theta = unif_(my_rand::rng) * 2.0 * PI;
                    double rand_r = unif_(my_rand::rng) * aperture_;
                    double jitter_y = unif_(my_rand::rng);
                    double jitter_x = unif_(my_rand::rng);

                    Vec3f origin2 = origin_ + vertical * std::cos(rand_theta) * rand_r + horizontal * std::sin(rand_theta) * rand_r;

                    pix_vec -= subpix_span_y * ((double)k - (double)subpix_[0]/2.0 + jitter_y) - subpix_span_x * ((double)l - (double)subpix_[1]/2.0 + jitter_x);
                    pix_vec -= origin2;
                    pix_vec = pix_vec.normalize();

                    Ray_t ray = Ray_t(origin2, pix_vec, Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);
                    ray.raycast(scene, max_bounces_, skybox_);
                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void RecCamAperture_t::focus(double focus_distance){
    focal_length_buffer_ = focus_distance;
}

void RecCamAperture_t::autoFocus(const Scene_t* scene, double position[2]){
    Vec3f horizontal, vertical;
    Vec3f ray_vec;
    Vec3f span_x, span_y;
    Shape_t* hit_obj = nullptr;
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    horizontal = direction_.cross(up_);
    vertical = horizontal.cross(direction_);
    ray_vec = origin_ + direction_ * focal_length_;
    span_y = vertical * focal_length_ * std::tan(fov_[0]/2.0) * 2.0;
    span_x = horizontal * focal_length_ * std::tan(fov_[1]/2.0) * 2.0;

    ray_vec -= span_y * (position[1] - 0.5) - span_x * (position[0] - 0.5);
    ray_vec -= origin_;

    Ray_t focus_ray = Ray_t(origin_, ray_vec.normalize(), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);
    
    scene->intersect(focus_ray, hit_obj, t, uv);

    if (t == std::numeric_limits<double>::infinity()){
        t = 1000000.0;
    }
    focus(t);
}