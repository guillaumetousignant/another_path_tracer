#ifndef REFERENTIALS_H
#define REFERENTIALS_H

#include "Vec3f.h"

Vec3f to_sph(const Vec3f &xyz);
Vec3f to_xyz(const Vec3f &sph);
Vec3f to_xyz_offset(const Vec3f &sph, const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3);

#endif