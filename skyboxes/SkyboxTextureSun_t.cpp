#include "SkyboxTextureSun_t.h"
#include "Texture_t.h"
#include <cmath>

#define PI 3.141592653589793238463

SkyboxTextureSun_t::SkyboxTextureSun_t(Texture_t* texture, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad) 
    : texture_(texture), sun_pos_{sun_pos[0], sun_pos[1]}, sun_col_(sun_col), sun_rad_(sun_rad) {}

SkyboxTextureSun_t::~SkyboxTextureSun_t(){}

Vec3f SkyboxTextureSun_t::get(const Vec3f &xyz) const{
    Vec3f sph = xyz.get_sph();
    double xy[2] = {sph[2]/(2.0 * PI) + 0.5, 1.0 - sph[1]/PI};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < (std::pow(sun_rad_, 2))) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}