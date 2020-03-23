#ifndef SKYBOX_T_H
#define SKYBOX_T_H

#include "entities/Vec3f.h"

namespace APTracer::Entities {
    class Skybox_t{
        public:
            Skybox_t(){};
            virtual ~Skybox_t(){};

            virtual Vec3f get(const Vec3f &xyz) const = 0;
    };
}

#endif