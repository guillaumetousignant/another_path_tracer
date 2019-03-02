#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"

Cam_t::Cam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image) {}

Cam_t::~Cam_t() {}

void Cam_t::update() {

}

void Cam_t::raytrace(Scene_t* scene) const {

}

void Cam_t::focus(double focus_distance) {

}

void Cam_t::write(std::string file_name /*= ""*/) {

}

void Cam_t::show() const {

}