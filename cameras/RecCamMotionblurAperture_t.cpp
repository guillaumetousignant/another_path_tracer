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
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), direction_last_(direction_), origin_last_(origin_), time_{time[0], time[1]}, up_last_(up_),
    focal_length_(focal_length), focal_length_last_(focal_length), aperture_(aperture), focal_length_buffer_(focal_length) {}

RecCamMotionblurAperture_t::~RecCamMotionblurAperture_t() {}

void RecCamMotionblurAperture_t::update() {
    origin_last_ = origin_;
    direction_last_ = direction_;
    up_last_ = up_;
    focal_length_last_ = focal_length_;

    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;
}

void RecCamMotionblurAperture_t::raytrace(const Scene_t* scene) {
    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();
    const Vec3f horizontal_last = direction_last_.cross(up_last_).normalize_inplace();
    const Vec3f vertical_last = horizontal_last.cross(direction_last_).normalize_inplace();
    const Vec3f focus_point = origin_ + direction_ * focal_length_;
    const Vec3f focus_point_last = origin_last_ + direction_last_ * focal_length_last_;
    const double tot_subpix = subpix_[0]*subpix_[1];
    const double pixel_span_y = focal_length_ * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    const double pixel_span_x = focal_length_ * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    const double pixel_span_y_last = focal_length_last_ * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    const double pixel_span_x_last = focal_length_last_ * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    
    const double subpix_span_y = pixel_span_y/subpix_[0];
    const double subpix_span_x = pixel_span_x/subpix_[1];
    const double subpix_span_y_last = pixel_span_y_last/subpix_[0];
    const double subpix_span_x_last = pixel_span_x_last/subpix_[1];

    #ifdef _WIN32
        int index; // Openmp on windows can't use unsigned index.
    #else
        unsigned int index;
    #endif

    image_->update();

    #pragma omp parallel for schedule(guided)
    for (index = 0; index < image_->size_y_ * image_->size_x_; ++index){
        const unsigned int i = index%image_->size_x_;
        const unsigned int j = index/image_->size_x_;
        Vec3f col = Vec3f(); // Or declare above?//std::cout << "noot " << i << " " << j << std::endl; // REMOVE
        
        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex){
            const unsigned int l = subindex%subpix_[1]; // x
            const unsigned int k = subindex/subpix_[1]; // y
            const double rand_time = unif_(my_rand::rng) * (time_[1] - time_[0]) + time_[0];
            const double rand_theta = unif_(my_rand::rng) * 2.0 * PI;
            const double rand_r = std::sqrt(unif_(my_rand::rng)) * aperture_;
            const double jitter_y = unif_(my_rand::rng);
            const double jitter_x = unif_(my_rand::rng);

            const Vec3f focus_point_int = focus_point * rand_time + focus_point_last * (1.0 - rand_time);
            const Vec3f horizontal_int = horizontal * rand_time + horizontal_last * (1.0 - rand_time);
            const Vec3f vertical_int = vertical * rand_time + vertical_last * (1.0 - rand_time);
            const Vec3f origin_int = origin_ * rand_time + origin_last_ * (1.0 - rand_time)
                                        + vertical_int * std::cos(rand_theta) * rand_r 
                                        + horizontal_int * std::sin(rand_theta) * rand_r;
            
            const double pixel_span_x_int = pixel_span_x * rand_time + pixel_span_x_last * (1.0 - rand_time);
            const double pixel_span_y_int = pixel_span_y * rand_time + pixel_span_y_last * (1.0 - rand_time);
            const double subpix_span_x_int = subpix_span_x * rand_time + subpix_span_x_last * (1.0 - rand_time);
            const double subpix_span_y_int = subpix_span_y * rand_time + subpix_span_y_last * (1.0 - rand_time);

            const Vec3f ray_vec = (focus_point_int - vertical_int * (pixel_span_y_int * ((double)j - (double)image_->size_y_/2.0 + 0.5) + subpix_span_y_int * ((double)k - (double)subpix_[0]/2.0 + jitter_y))
                            + horizontal_int * (pixel_span_x_int * ((double)i - (double)image_->size_x_/2.0 + 0.5) + subpix_span_x_int * ((double)l - (double)subpix_[1]/2.0 + jitter_x))
                            - origin_int).normalize_inplace();

            Ray_t ray = Ray_t(origin_int, ray_vec, Vec3f(), Vec3f(1.0), medium_list_, rand_time);
            ray.raycast(scene, max_bounces_, skybox_);

            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);
    }
}

void RecCamMotionblurAperture_t::focus(double focus_distance){
    focal_length_buffer_ = focus_distance;
}

void RecCamMotionblurAperture_t::autoFocus(const Scene_t* scene, const double (&position)[2]){
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    const Vec3f horizontal = direction_.cross(up_);
    const Vec3f vertical = horizontal.cross(direction_);
    const Vec3f focus_point = origin_ + direction_ * focal_length_;
    const Vec3f span_x = horizontal * focal_length_ * std::tan(fov_[1]/2.0) * 2.0;
    const Vec3f span_y = vertical * focal_length_ * std::tan(fov_[0]/2.0) * 2.0;

    const Vec3f ray_vec = (focus_point - span_y * (position[1] - 0.5) + span_x * (position[0] - 0.5) - origin_).normalize_inplace();

    const Ray_t focus_ray = Ray_t(origin_, ray_vec, Vec3f(), Vec3f(1.0), medium_list_);

    if (scene->intersect(focus_ray, t, uv) == nullptr){
        t = 1000000.0;
    }
    focus(t);
}

void RecCamMotionblurAperture_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name);
}

void RecCamMotionblurAperture_t::show() const {
    // What to do here?
}

void RecCamMotionblurAperture_t::reset(){
    image_->reset();
}