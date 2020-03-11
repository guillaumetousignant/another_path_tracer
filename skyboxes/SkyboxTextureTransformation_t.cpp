#include "SkyboxTextureTransformation_t.h"
#include "Texture_t.h"
#include "TransformMatrix_t.h"

#define PI 3.141592653589793238463

SkyboxTextureTransformation_t::SkyboxTextureTransformation_t(Texture_t* texture, TransformMatrix_t* transformation) 
    : texture_(texture), transformation_(transformation) {}

SkyboxTextureTransformation_t::~SkyboxTextureTransformation_t(){}

Vec3f SkyboxTextureTransformation_t::get(const Vec3f &xyz) const{
    const Vec3f sph = transformation_->transformDir().multDir(xyz).to_sph(); // CHECK cache transformation_->transformDir()??
    const double xy[2] = {sph[2]/(2.0 * PI) + 0.5, 1.0 - sph[1]/PI};
    return texture_->get(xy);
}