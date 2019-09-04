#include "SkyboxTexture_t.h"
#include "Texture_t.h"

#define PI 3.141592653589793238463

SkyboxTexture_t::SkyboxTexture_t(Texture_t* texture) : texture_(texture) {}

SkyboxTexture_t::~SkyboxTexture_t(){}

Vec3f SkyboxTexture_t::get(const Vec3f &xyz) const{
    const Vec3f sph = xyz.get_sph();
    const double xy[2] = {sph[2]/(2.0 * PI) + 0.5, 1.0 - sph[1]/PI};
    return texture_->get(xy);
}