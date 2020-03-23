#ifndef MESHGEOMETRY_T_H
#define MESHGEOMETRY_T_H

#include <string>
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class MeshGeometry_t{
        public:
            MeshGeometry_t(const std::string &filename);
            ~MeshGeometry_t();

            unsigned int n_tris_;
            std::string* mat_;
            Vec3f* v_;
            double** vt_; // Is this legal?
            Vec3f* vn_;
        
        private:
            void readObj(const std::string &filename);
            void readSU2(const std::string &filename);
            void deNan();
    };
}}

#endif