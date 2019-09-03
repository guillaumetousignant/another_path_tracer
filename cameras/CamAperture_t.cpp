#include "CamAperture_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "RandomGenerator_t.h"
#include <limits>

#define PI 3.141592653589793238463

CamAperture_t::CamAperture_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind),
    image_(image), unif_(0.0, 1.0), focal_length_(focal_length), aperture_(aperture), focal_length_buffer_(focal_length) {}

CamAperture_t::~CamAperture_t() {}

void CamAperture_t::update() {
    origin_ = transformation_->multVec(Vec3f());
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;
}

void CamAperture_t::raytrace(const Scene_t* scene) {
    const double tot_subpix = subpix_[0]*subpix_[1];
    const double pixel_span_y = fov_[0]/image_->size_y_;
    const double pixel_span_x = fov_[1]/image_->size_x_;
    const double subpix_span_y = pixel_span_y/subpix_[0];
    const double subpix_span_x = pixel_span_x/subpix_[1];
    const Vec3f horizontal = direction_.cross(up_).normalize();
    const Vec3f vertical = horizontal.cross(direction_).normalize();

    image_->update();

    #pragma omp parallel for collapse(2) schedule(guided)
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            const Vec3f pix_vec = Vec3f(1.0, PI/2.0 + ((double)j - (double)image_->size_y_/2.0 + 0.5)*pixel_span_y, ((double)i - (double)image_->size_x_/2.0 + 0.5)*pixel_span_x);
            
            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    
                    const double rand_theta = unif_(my_rand::rng) * 2.0 * PI;
                    const double rand_r = std::sqrt(unif_(my_rand::rng)) * aperture_;
                    const double jitter_y = unif_(my_rand::rng);
                    const double jitter_x = unif_(my_rand::rng);

                    Vec3f subpix_vec = pix_vec + Vec3f(0.0, ((double)k - (double)subpix_[0]/2.0 + jitter_y)*subpix_span_y, ((double)l - (double)subpix_[1]/2.0 + jitter_x)*subpix_span_x);
                    const Vec3f origin2 = origin_ + vertical * std::cos(rand_theta) * rand_r + horizontal * std::sin(rand_theta) * rand_r;
                    
                    subpix_vec = (origin_ + subpix_vec.to_xyz_offset(direction_, horizontal, vertical) * focal_length_ - origin2).normalize();

                    Ray_t ray = Ray_t(origin2, subpix_vec, Vec3f(), Vec3f(1.0), medium_list_);
                    ray.raycast(scene, max_bounces_, skybox_);
                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void CamAperture_t::focus(double focus_distance){
    focal_length_buffer_ = focus_distance;
}

void CamAperture_t::autoFocus(const Scene_t* scene, const double (&position)[2]){
    Shape_t* hit_obj = nullptr;
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    const Vec3f horizontal = direction_.cross(up_);
    const Vec3f vertical = horizontal.cross(direction_);

    const Vec3f ray_direction_sph = Vec3f(1.0, PI/2.0 + (position[1]-0.5)*fov_[0], (position[0]-0.5)*fov_[1]).to_xyz_offset(direction_, horizontal, vertical); // 0, y, x

    const Ray_t focus_ray = Ray_t(origin_, ray_direction_sph, Vec3f(), Vec3f(1.0), medium_list_);

    scene->intersect(focus_ray, hit_obj, t, uv);

    if (t == std::numeric_limits<double>::infinity()){
        t = 1000000.0;
    }
    focus(t);
}

void CamAperture_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name, gammaind_);
}

void CamAperture_t::show() const {
    // What to do here?
}

void CamAperture_t::reset(){
    image_->reset();
}