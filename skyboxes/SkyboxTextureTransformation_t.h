#ifndef SKYBOXTEXTURETRANSFORMATION_T_H
#define SKYBOXTEXTURETRANSFORMATION_T_H

#include "SkyboxTexture_t.h"
#include "Vec3f.h"

class Texture_t;
class TransformMatrix_t;

class SkyboxTextureTransformation_t : public SkyboxTexture_t{
    public:
        SkyboxTextureTransformation_t(Texture_t* texture, TransformMatrix_t* transformation);
        virtual ~SkyboxTextureTransformation_t();

        TransformMatrix_t* transformation_;
        
        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif