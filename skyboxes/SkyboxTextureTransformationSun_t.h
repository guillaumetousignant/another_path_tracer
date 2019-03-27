#ifndef SKYBOXTEXTURETRANSFORMATIONSUN_T_H
#define SKYBOXTEXTURETRANSFORMATIONSUN_T_H

#include "SkyboxTextureTransformation_t.h"
#include "Vec3f.h"

class Texture_t;
class TransformMatrix_t;

class SkyboxTextureTransformationSun_t : public SkyboxTextureTransformation_t{
    public:
        SkyboxTextureTransformationSun_t(Texture_t* texture, TransformMatrix_t* transformation, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad);
        ~SkyboxTextureTransformationSun_t();

        double sun_pos_[2];
        Vec3f sun_col_;
        double sun_rad_;
        
        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif