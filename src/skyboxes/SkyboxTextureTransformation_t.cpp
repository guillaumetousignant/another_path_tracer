#include "skyboxes/SkyboxTextureTransformation_t.h"
#include "entities/Texture_t.h"
#include "entities/TransformMatrix_t.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureTransformation_t::SkyboxTextureTransformation_t(APTracer::Entities::Texture_t* texture, APTracer::Entities::TransformMatrix_t* transformation) 
    : texture_(texture), transformation_(transformation) {}

APTracer::Skyboxes::SkyboxTextureTransformation_t::~SkyboxTextureTransformation_t() = default;

Vec3f APTracer::Skyboxes::SkyboxTextureTransformation_t::get(const Vec3f &xyz) const {
    const Vec3f sph = transformation_->transformDir().multDir(xyz).to_sph(); // CHECK cache transformation_->transformDir()??
    const double xy[2] = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};
    return texture_->get(xy);
}