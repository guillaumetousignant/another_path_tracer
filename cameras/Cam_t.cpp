#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"

Cam_t::Cam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image) {
    direction_ = Vec3f(0, 1, 0); // CHECK fill when transformation mats are done
    origin_ = Vec3f(0, 0, 0);
}

Cam_t::~Cam_t() {}

void Cam_t::update() {
    // CHECK fill when transforms are done
}

void Cam_t::raytrace(Scene_t* scene) const {

}

void Cam_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty){
        file_name = filename_;
    }
    // Call imgbuffer write
}

void Cam_t::show() const {
    // What to do here?
}