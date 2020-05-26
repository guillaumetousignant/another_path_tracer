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

            /**
             * @brief Rotates the matrix around the x axis by a specified angle in radians.
             * 
             * This means that if an object it at (1, 0, 0) and is rotated π rad, it will be at (-1, 0, 0) and face the opposite direction.
             * 
             * @param angle Angle in radians to rotate around the y axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateYAxis(double angle);

            /**
             * @brief Rotates the matrix around the z axis by a specified angle in radians.
             * 
             * This means that if an object it at (1, 0, 0) and is rotated π rad, it will be at (-1, 0, 0) and face the opposite direction.
             * 
             * @param angle Angle in radians to rotate around the z axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateZAxis(double angle);

            /**
             * @brief Rotates the matrix on itself around the x direction.
             * 
             * The object won't move but will face the opposite direction around x.
             * 
             * @param angle Angle in radians to rotate on itself around the x direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateX(double angle);

            /**
             * @brief Rotates the matrix on itself around the y direction.
             * 
             * The object won't move but will face the opposite direction around y.
             * 
             * @param angle Angle in radians to rotate on itself around the y direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateY(double angle);

            /**
             * @brief Rotates the matrix on itself around the z direction.
             * 
             * The object won't move but will face the opposite direction around z.
             * 
             * @param angle Angle in radians to rotate on itself around the z direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateZ(double angle);

            /**
             * @brief Rotates the matrix around an arbitrary axis by a specified angle in radians.
             * 
             * This rotates the matrix around an arbitrary axis passing through (0, 0, 0).
             * This moves the object.
             * 
             * @param vec Axis around which the matrix will be rotated. Passes through (0, 0, 0).
             * @param angle Angle in radians to rotate around the axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotateAxis(const Vec3f &vec, double angle);

            /**
             * @brief Rotates the matrix on itself around an arbitrary axis by a specified angle in radians.
             * 
             * This rotates the matrix around an arbitrary axis passing through the matrix's center.
             * This doesn't move the object.
             * 
             * @param vec Axis around which the matrix will be rotated on itself.
             * @param angle Angle in radians to rotate on itself around the axis' direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& rotate(const Vec3f &vec, double angle);

            /**
             * @brief Translates the matrix in 3D space.
             * 
             * @param vec Vector by which to translate the matrix.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& translate(const Vec3f &vec);

            /**
             * @brief Scales the matrix from (0, 0, 0) by a three-component vector.
             * 
             * This means that if an object is at (1, 0, 1) and is scaled by (2, 2, 1) it will
             * be placed at (2, 0, 1) and be twice bigger except for the z direction.
             * 
             * @param vec Vector whose three components are the scale factors for the three axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& scaleAxis(const Vec3f &vec);

            /**
             * @brief Scales the matrix from (0, 0, 0) by a factor.
             * 
             * This means that if an object is at (1, 0, 1) and is scaled by 2 it will
             * be placed at (2, 0, 2) and be twice bigger.
             * 
             * @param fac Factor by which the matrix will be scaled from (0, 0, 0).
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            TransformMatrix_t& scaleAxis(double fac);

            /**
             * @brief Scales the matrix inplace by a three-component vector.
             * 
             * This won't move the object.
             * 
             * @param vec Vector whose three components are the scale factors for the three axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
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