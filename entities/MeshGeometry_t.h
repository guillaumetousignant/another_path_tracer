#ifndef MESHGEOMETRY_T_H
#define MESHGEOMETRY_T_H

#include <string>
#include "Vec3f.h"

class MeshGeometry_t{
    public:
        MeshGeometry_t();
        ~MeshGeometry_t();

        unsigned int n_tris_;
        std::string* mat_;
        Vec3f* v_;
        double* vt_[2]; // Is this legal?
        Vec3f* vn_;
};

#endif