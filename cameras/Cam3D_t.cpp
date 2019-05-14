#include "Cam3D_t.h"
#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "Referentials.h"
#include "RandomGenerator_t.h"

#define PI 3.141592653589793238463

Cam3D_t::Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind) :
    Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), eye_dist_(eye_dist), focal_length_(focal_length), focal_length_buffer_(focal_length) {

    std::string filename_S, filename_L, filename_R;
    size_t point;

    point = filename.find_last_of(".");

    if (point != std::string::npos){
        filename_L = filename.substr(0, point) + "_L" + filename.substr(point);
        filename_R = filename.substr(0, point) + "_R" + filename.substr(point);
        filename_S = filename;
    }
    else{
        filename_L = filename + "_L.png";
        filename_R = filename + "_R.png";
        filename_S = filename;
    }

    camera_L_ = new Cam_t(transformation, filename_L, up_, fov_, subpix_, image_L, medium_list_, skybox_, max_bounces_, gammaind_);
    camera_R_ = new Cam_t(transformation, filename_R, up_, fov_, subpix_, image_R, medium_list_, skybox_, max_bounces_, gammaind_);

    filename_ = filename_S;
    image_ = image;
}

Cam3D_t::~Cam3D_t() {
    delete camera_L_;
    delete camera_R_;
}

void Cam3D_t::update() {
    origin_ = transformation_->multVec(Vec3f(0.0, 0.0, 0.0));
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
}

void Cam3D_t::raytrace(const Scene_t* scene) {
    double tot_subpix;
    double pixel_span_x, pixel_span_y;
    double subpix_span_x, subpix_span_y;
    Vec3f horizontal, vertical;
    
    tot_subpix = subpix_[0]*subpix_[1];
    pixel_span_y = fov_[0]/image_->size_y_;
    pixel_span_x = fov_[1]/image_->size_x_;
    subpix_span_y = pixel_span_y/subpix_[0];
    subpix_span_x = pixel_span_x/subpix_[1];
    horizontal = direction_.cross(up_).normalize();
    vertical = horizontal.cross(direction_).normalize();

    image_->update();

    #pragma omp parallel for collapse(2) schedule(guided)
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            Vec3f pix_vec = Vec3f(1.0, PI/2.0 + ((double)j - (double)image_->size_y_/2.0 + 0.5)*pixel_span_y, ((double)i - (double)image_->size_x_/2.0 + 0.5)*pixel_span_x); // Is shit after this line
            
            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    
                    double jitter_y = unif_(my_rand::rng);
                    double jitter_x = unif_(my_rand::rng);

                    Vec3f subpix_vec = pix_vec + Vec3f(0.0, ((double)k - (double)subpix_[0]/2.0 + jitter_y)*subpix_span_y, ((double)l - (double)subpix_[1]/2.0 + jitter_x)*subpix_span_x); // Is shit after this line
                    subpix_vec = to_xyz_offset(subpix_vec, direction_, horizontal, vertical);

                    Ray_t ray = Ray_t(origin_, subpix_vec, Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);
                    ray.raycast(scene, max_bounces_, skybox_);
                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void Cam3D_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name);
}

void Cam3D_t::show() const {
    // What to do here?
}

void Cam3D_t::reset(){
    image_->reset();
}

void Cam3D_t::focus(double focus_distance){

}

void Cam3D_t::autoFocus(const Scene_t* scene, const double (&position)[2]){

}