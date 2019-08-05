#ifndef SKYBOXTEXTURETRANSFORMATION_T_H
#define SKYBOXTEXTURETRANSFORMATION_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class Texture_t;
class TransformMatrix_t;

class SkyboxTextureTransformation_t : public Skybox_t{
    public:
        SkyboxTextureTransformation_t(Texture_t* texture, TransformMatrix_t* transformation);
        virtual ~SkyboxTextureTransformation_t();

        Texture_t* texture_;
        TransformMatrix_t* transformation_;
        
        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif