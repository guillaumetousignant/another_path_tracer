#include "SkyboxTextureTransformationSun_t.h"
#include "Texture_t.h"
#include "TransformMatrix_t.h"

#define PI 3.141592653589793238463

SkyboxTextureTransformationSun_t::SkyboxTextureTransformationSun_t(Texture_t* texture, TransformMatrix_t* transformation, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad) 
    : texture_(texture), transformation_(transformation), sun_pos_{sun_pos[0], sun_pos[1]}, sun_col_(sun_col), sun_rad_(sun_rad) {}

SkyboxTextureTransformationSun_t::~SkyboxTextureTransformationSun_t(){}

Vec3f SkyboxTextureTransformationSun_t::get(const Vec3f &xyz) const{
    TransformMatrix_t transform_norm = transformation_->transformDir(); // CHECK do this on transformation itself?

    Vec3f sph = transform_norm.multDir(xyz).to_sph();
    double xy[2] = {sph[2]/(2.0 * PI) + 0.5, 1.0 - sph[1]/PI};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < (std::pow(sun_rad_, 2))) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}