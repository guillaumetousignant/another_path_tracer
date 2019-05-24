#ifndef SKYBOXFLATSUN_T_H
#define SKYBOXFLATSUN_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

class DirectionalLight_t;

class SkyboxFlatSun_t : public Skybox_t{
    public:
        SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t* light);
        SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t** lights, unsigned int n_lights);
        virtual ~SkyboxFlatSun_t();

        Vec3f background_;
        DirectionalLight_t** lights_;
        unsigned int n_lights_;

        virtual Vec3f get(const Vec3f &xyz) const;
};
#endif