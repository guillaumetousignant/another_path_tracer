#include "skyboxes/SkyboxTextureTransformationSun_t.h"
#include "entities/Texture_t.h"
#include "entities/TransformMatrix_t.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureTransformationSun_t::SkyboxTextureTransformationSun_t(APTracer::Entities::Texture_t* texture, APTracer::Entities::TransformMatrix_t* transformation, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad) 
    : texture_(texture), transformation_(transformation), sun_pos_{sun_pos[0], sun_pos[1]}, sun_col_(sun_col), sun_rad_(sun_rad) {}

APTracer::Skyboxes::SkyboxTextureTransformationSun_t::~SkyboxTextureTransformationSun_t() {}

Vec3f APTracer::Skyboxes::SkyboxTextureTransformationSun_t::get(const Vec3f &xyz) const {
    const Vec3f sph = transformation_->transformDir().multDir(xyz).to_sph(); // CHECK cache transformation_->transformDir()??
    const double xy[2] = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < (std::pow(sun_rad_, 2))) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}