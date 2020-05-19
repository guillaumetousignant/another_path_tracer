#ifndef TRANSFORMMATRIX_T_H
#define TRANSFORMMATRIX_T_H

#include "entities/Vec3f.h"
#include <cmath>

namespace APTracer { namespace Entities {
    
    /**
     * @brief The transformation matrix class represents a 4x4 3D transformation matrix, used to transform other objects in 3D space.
     * 
     * The transformation matrix can be rotated, scaled, translated and other operations. Other objects, such as shapes and rays,
     * then use the transformation matrix to modify themselves. All objects using transformation matrices can therefore be modified
     * in a single unified way, without modifying the underlying objects. This enables basic instancing.
     */
    class TransformMatrix_t{
        public:
            /**
             * @brief Construct a new TransformMatrix_t object as the identity transformation matrix, which does not modify objects.
             */
            TransformMatrix_t();

            /**
             * @brief Construct a new TransformMatrix_t object from the 16 values of its 4x4 matrix.
             * 
             * Elements are given row by row. The identity matrix has ones on its diagonal.
             * 
             * @param i0 Element at position (1, 1)
             * @param i1 Element at position (1, 2)
             * @param i2 Element at position (1, 3)
             * @param i3 Element at position (1, 4)
             * @param i4 Element at position (2, 1)
             * @param i5 Element at position (2, 2)
             * @param i6 Element at position (2, 3)
             * @param i7 Element at position (2, 4)
             * @param i8 Element at position (3, 1)
             * @param i9 Element at position (3, 2)
             * @param i10 Element at position (3, 3)
             * @param i11 Element at position (3, 4)
             * @param i12 Element at position (4, 1)
             * @param i13 Element at position (4, 2)
             * @param i14 Element at position (4, 3)
             * @param i15 Element at position (4, 4)
             */
            TransformMatrix_t(double i0, double i1, double i2, double i3, double i4, double i5, double i6, double i7, double i8, double i9, double i10, double i11, double i12, double i13, double i14, double i15);

            /**
             * @brief Construct a new TransformMatrix_t object from another transformation matrix, copying it.
             * 
             * @param other Transformation matrix to be copied into the new transformation matrix.
             */
            TransformMatrix_t(const TransformMatrix_t &other);

            /**
             * @brief Destroy the TransformMatrix_t object.
             */
            ~TransformMatrix_t();

            double matrix_[16]; /**< @brief Array of the 16 values in the 4x4 matrix.*/

            /**
             * @brief Rotates the matrix around the x axis by a specified angle in radians.
             * 
             * This means that if an object it at (0, 1, 0) and is rotated π rad, it will be at (0, -1, 0) and face the opposite direction.
             * 
             * @param angle Angle in radians to rotate around the x axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
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