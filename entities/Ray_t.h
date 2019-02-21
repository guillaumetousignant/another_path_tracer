#ifndef RAY_T_H
#define RAY_T_H

#include "Vec3f.h"
#include "Medium_t.h"
#include <list>

class Ray_t{
    public:
        Ray_t(){};
        ~Ray_t();

        Vec3f origin_;
        Vec3f direction_;
        Vec3f mask_;
        Vec3f colour_;
        double dist_;
        std::list <Medium_t> medium_list_;
        double time_[2];
};

#endif