#include "RecCamMotionblurAperture_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define PI 3.141592653589793238463

RecCamMotionblurAperture_t::RecCamMotionblurAperture_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double (&time)[2], double gammaind) 
    : RecCamMotionblur_t(transformation, filename, up, fov, subpix, image, medium_list, skybox, max_bounces, time, gammaind), 
    focal_length_(focal_length), focal_length_last_(focal_length), aperture_(aperture), focal_length_buffer_(focal_length) {}

RecCamMotionblurAperture_t::~RecCamMotionblurAperture_t() {}

void RecCamMotionblurAperture_t::update() {
    origin_last_ = origin_;
    direction_last_ = direction_;
    up_last_ = up_;
    focal_length_last_ = focal_length_;

    origin_ = transformation_->multVec(Vec3f(0.0, 0.0, 0.0));
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;
}

void RecCamMotionblurAperture_t::raytrace(const Scene_t* scene) {
    double tot_subpix;
    double pixel_span_x, pixel_span_y, pixel_span_x_last, pixel_span_y_last;
    double subpix_span_x, subpix_span_y, subpix_span_x_last, subpix_span_y_last;
    Vec3f horizontal, vertical, horizontal_last, vertical_last;
    Vec3f focus_point, focus_point_last;
    
    horizontal = direction_.cross(up_).normalize();
    vertical = horizontal.cross(direction_).normalize();
    horizontal_last = direction_last_.cross(up_last_).normalize();
    vertical_last = horizontal_last.cross(direction_last_).normalize();
    focus_point = origin_ + direction_ * focal_length_;
    focus_point_last = origin_last_ + direction_last_ * focal_length_last_;
    tot_subpix = subpix_[0]*subpix_[1];
    pixel_span_y = focal_length_ * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    pixel_span_x = focal_length_ * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    pixel_span_y_last = focal_length_last_ * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    pixel_span_x_last = focal_length_last_ * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    
    subpix_span_y = pixel_span_y/subpix_[0];
    subpix_span_x = pixel_span_x/subpix_[1];
    subpix_span_y_last = pixel_span_y_last/subpix_[0];
    subpix_span_x_last = pixel_span_x_last/subpix_[1];

    image_->update();

    #pragma omp parallel for collapse(2) schedule(guided)
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?//std::cout << "noot " << i << " " << j << std::endl; // REMOVE
            
            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    double rand_time = unif_(my_rand::rng) * (time_[1] - time_[0]) + time_[0];
                    double rand_theta = unif_(my_rand::rng) * 2.0 * PI;
                    double rand_r = unif_(my_rand::rng) * aperture_;
                    double jitter_y = unif_(my_rand::rng);
                    double jitter_x = unif_(my_rand::rng);

                    Vec3f focus_point_int = focus_point * rand_time + focus_point_last * (1.0 - rand_time);
                    Vec3f horizontal_int = horizontal * rand_time + horizontal_last * (1.0 - rand_time);
                    Vec3f vertical_int = vertical * rand_time + vertical_last * (1.0 - rand_time);
                    Vec3f origin_int = origin_ * rand_time + origin_last_ * (1.0 - rand_time);
                    
                    double pixel_span_x_int = pixel_span_x * rand_time + pixel_span_x_last * (1.0 - rand_time);
                    double pixel_span_y_int = pixel_span_y * rand_time + pixel_span_y_last * (1.0 - rand_time);
                    double subpix_span_x_int = subpix_span_x * rand_time + subpix_span_x_last * (1.0 - rand_time);
                    double subpix_span_y_int = subpix_span_y * rand_time + subpix_span_y_last * (1.0 - rand_time);

                    origin_int += vertical_int * std::cos(rand_theta) * rand_r + horizontal_int * std::sin(rand_theta) * rand_r;

                    Vec3f ray_vec = focus_point_int - vertical_int * (pixel_span_y_int * ((double)j - (double)image_->size_y_/2.0 + 0.5) + subpix_span_y_int * ((double)k - (double)subpix_[0]/2.0 + jitter_y))
                                 + horizontal_int * (pixel_span_x_int * ((double)i - (double)image_->size_x_/2.0 + 0.5) + subpix_span_x_int * ((double)l - (double)subpix_[1]/2.0 + jitter_x))
                                 - origin_int;

                    Ray_t ray = Ray_t(origin_int, ray_vec.normalize(), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_, rand_time);
                    ray.raycast(scene, max_bounces_, skybox_);

                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void RecCamMotionblurAperture_t::focus(double focus_distance){
    focal_length_buffer_ = focus_distance;
}

void RecCamMotionblurAperture_t::autoFocus(const Scene_t* scene, const double (&position)[2]){
    Vec3f horizontal, vertical;
    Vec3f focus_point;
    Vec3f span_x, span_y;
    Vec3f ray_vec;
    Shape_t* hit_obj = nullptr;
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    horizontal = direction_.cross(up_);
    vertical = horizontal.cross(direction_);
    focus_point = origin_ + direction_ * focal_length_;
    span_x = horizontal * focal_length_ * std::tan(fov_[1]/2.0) * 2.0;
    span_y = vertical * focal_length_ * std::tan(fov_[0]/2.0) * 2.0;

    ray_vec = focus_point - span_y * (position[1] - 0.5) + span_x * (position[0] - 0.5) - origin_;

    Ray_t focus_ray = Ray_t(origin_, ray_vec.normalize(), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);

    scene->intersect(focus_ray, hit_obj, t, uv);

    if (t == std::numeric_limits<double>::infinity()){
        t = 1000000.0;
    }
    focus(t);
}