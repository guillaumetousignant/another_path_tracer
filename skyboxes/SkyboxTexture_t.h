#ifndef SKYBOXTEXTURE_T_H
#define SKYBOXTEXTURE_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class Texture_t;

class SkyboxTexture_t final : public Skybox_t{
    public:
        SkyboxTexture_t(Texture_t* texture);
        virtual ~SkyboxTexture_t() final;

        Texture_t* texture_;

        virtual Vec3f get(const Vec3f &xyz) const final;
};
#endif