#ifndef SKYBOXFLAT_T_H
#define SKYBOXFLAT_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class SkyboxFlat_t : public Skybox_t{
    public:
        SkyboxFlat_t(Vec3f background);
        ~SkyboxFlat_t();

        Vec3f background_;

        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif