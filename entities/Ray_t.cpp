#include "Ray_t.h"

Ray_t::Ray_t(Vec3f origin, Vec3f direction, Vec3f colour, Vec3f mask, std::list<Medium_t*> medium_list, double time = 0) : 
    origin_(origin), direction_(direction), colour_(colour), mask_(mask), dist_(0), medium_list_(medium_list), time_(time) {};

Ray_t::~Ray_t(){};

void Ray_t::raycast(Scene_t* scene, unsigned int max_bounces, Skybox_t* skybox){

};

void Ray_t::add_to_mediums(Medium_t* medium){

};

void Ray_t::remove_from_mediums(Medium_t* medium){

};