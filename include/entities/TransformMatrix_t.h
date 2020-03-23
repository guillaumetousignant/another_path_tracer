#ifndef TRANSFORMMATRIX_T_H
#define TRANSFORMMATRIX_T_H

#include "entities/Vec3f.h"
#include <cmath>

namespace APTracer { namespace Entities {
    class TransformMatrix_t{
        public:
            TransformMatrix_t();
            TransformMatrix_t(double i0, double i1, double i2, double i3, double i4, double i5, double i6, double i7, double i8, double i9, double i10, double i11, double i12, double i13, double i14, double i15);
            TransformMatrix_t(const TransformMatrix_t &other);
            ~TransformMatrix_t();

            double matrix_[16];

            TransformMatrix_t& rotateXAxis(double angle);
            TransformMatrix_t& rotateYAxis(double angle);
            TransformMatrix_t& rotateZAxis(double angle);
            TransformMatrix_t& rotateX(double angle);
            TransformMatrix_t& rotateY(double angle);
            TransformMatrix_t& rotateZ(double angle);
            TransformMatrix_t& rotateAxis(const Vec3f &vec, double angle);
            TransformMatrix_t& rotate(const Vec3f &vec, double angle);
            TransformMatrix_t& translate(const Vec3f &vec);
            TransformMatrix_t& scaleAxis(const Vec3f &vec);
            TransformMatrix_t& scaleAxis(double fac);
            TransformMatrix_t& scale(const Vec3f &vec);
            TransformMatrix_t& scale(double fac);
            TransformMatrix_t& reflect(const Vec3f &vec);
            TransformMatrix_t& shear(const Vec3f &vec);
            TransformMatrix_t& transpose();
            TransformMatrix_t& invert();
            TransformMatrix_t& neg();
            Vec3f multVec(const Vec3f &vec) const;
            Vec3f multDir(const Vec3f &vec) const;
            TransformMatrix_t transformDir() const; // Should return reference?
            double getScale() const;
    };
}}

#endif