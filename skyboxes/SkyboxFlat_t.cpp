#include "SkyboxFlat_t.h"

SkyboxFlat_t::SkyboxFlat_t(Vec3f background) : background_(background) {}

SkyboxFlat_t::~SkyboxFlat_t(){}

Vec3f SkyboxFlat_t::get(const Vec3f &xyz) const{
    return background_;
}