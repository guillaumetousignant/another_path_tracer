#ifndef SKYBOXTEXTURE_T_H
#define SKYBOXTEXTURE_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class Texture_t;

class SkyboxTexture_t : public Skybox_t{
    public:
        SkyboxTexture_t(Texture_t* texture);
        ~SkyboxTexture_t();

        Texture_t* texture_;

        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif