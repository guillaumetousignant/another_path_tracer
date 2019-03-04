#ifndef TRANSFORMMATRIX_T_H
#define TRANSFORMMATRIX_T_H

#include "Vec3f.h"

class TransformMatrix_t{
    public:
        TransformMatrix_t();
        ~TransformMatrix_t();

        void rotateXAxis(double angle);
        void rotateYAxis(double angle);
        void rotateZAxis(double angle);
        void rotateX(double angle);
        void rotateY(double angle);
        void rotateZ(double angle);
        void rotateAxis(const Vec3f &vec, double angle);
        void rotate(const Vec3f &vec, double angle);
        void translate(const Vec3f &vec);
        void scaleAxis(const Vec3f &vec);
        void scaleAxis(double fac);
        void scale(const Vec3f &vec);
        void scale(double fac);
        void reflect(const Vec3f &vec);
        void shear(const Vec3f &vec);
        void transpose();
        void invert();
        void neg();
        Vec3f multVec(const Vec3f &vec);
        Vec3f multDir(const Vec3f &vec);
        TransformMatrix_t transformDir();
};

#endif