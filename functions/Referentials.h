#ifndef REFERENTIALS_H
#define REFERENTIALS_H

#include "Vec3f.h"
#include <math.h>

Vec3f to_sph(const Vec3f &xyz) {
    double r = xyz.magnitude;
    return Vec3f(r, acos(xyz[2]/r), atan2(xyz[1], xyz[0])); //CHECK acos might return NANs?
}

Vec3f to_xyz(const Vec3f &sph) {
    return Vec3f(sph[0]*sin(sph[1])*cos(sph[2]), sph[0]*sin(sph[1])*sin(sph[2]), sph[0]*cos(sph[1]));
}
#endif