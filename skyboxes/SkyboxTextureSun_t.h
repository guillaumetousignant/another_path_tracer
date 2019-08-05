#ifndef SKYBOXTEXTURESUN_T_H
#define SKYBOXTEXTURESUN_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class Texture_t;

class SkyboxTextureSun_t : public Skybox_t{
    public:
        SkyboxTextureSun_t(Texture_t* texture, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad);
        virtual ~SkyboxTextureSun_t();

        Texture_t* texture_;
        double sun_pos_[2];
        Vec3f sun_col_;
        double sun_rad_;

        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif