#include "skyboxes/SkyboxTextureSun_t.h"
#include "entities/Texture_t.h"
#include <cmath>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureSun_t::SkyboxTextureSun_t(APTracer::Entities::Texture_t* texture, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad) 
    : texture_(texture), sun_pos_{sun_pos[0], sun_pos[1]}, sun_col_(sun_col), sun_rad_(sun_rad) {}

APTracer::Skyboxes::SkyboxTextureSun_t::~SkyboxTextureSun_t() = default;

Vec3f APTracer::Skyboxes::SkyboxTextureSun_t::get(const Vec3f &xyz) const {
    const Vec3f sph = xyz.get_sph();
    const double xy[2] = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < std::pow(sun_rad_, 2)) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}