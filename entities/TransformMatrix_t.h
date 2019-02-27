#ifndef TRANSFORMMATRIX_T_H
#define TRANSFORMMATRIX_T_H

#include "Vec3f.h"

class TransformMatrix_t{
    public:
        TransformMatrix_t(){};
        ~TransformMatrix_t(){};

        void rotateXAxis(double angle){};
        void rotateYAxis(double angle){};
        void rotateZAxis(double angle){};
        void rotateX(double angle){};
        void rotateY(double angle){};
        void rotateZ(double angle){};
        void rotateAxis(Vec3f vec, double angle){};
        void rotate(Vec3f vec, double angle){};
        void translate(Vec3f vec){};
        void scaleAxis(Vec3f vec){};
        void scaleAxis(double fac){};
        void scale(Vec3f vec){};
        void scale(double fac){};
        void reflect(Vec3f vec){};
        void shear(Vec3f vec){};
        void transpose(){};
        void invert(){};
        void neg(){};
        Vec3f multVec(Vec3f vec){};
        Vec3f multDir(Vec3f vec){};
        TransformMatrix_t transformDir(){};
};

#endif