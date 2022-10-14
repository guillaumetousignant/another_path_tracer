#ifndef APTRACER_ENTITIES_TRANSFORMMATRIX_T_H
#define APTRACER_ENTITIES_TRANSFORMMATRIX_T_H

#include "entities/Vec3f.h"
#include <array>
#include <cmath>

namespace APTracer { namespace Entities {

    /**
     * @brief The transformation matrix class represents a 4x4 3D transformation matrix, used to transform other objects in 3D space.
     *
     * The transformation matrix can be rotated, scaled, translated and other operations. Other objects, such as shapes and rays,
     * then use the transformation matrix to modify themselves. All objects using transformation matrices can therefore be modified
     * in a single unified way, without modifying the underlying objects. This enables basic instancing.
     */
    class TransformMatrix_t {
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
            TransformMatrix_t(
                double i0, double i1, double i2, double i3, double i4, double i5, double i6, double i7, double i8, double i9, double i10, double i11, double i12, double i13, double i14, double i15);

            /**
             * @brief Construct a new TransformMatrix_t object from an array of the 16 values of its 4x4 matrix.
             *
             * @param values Array containing the elements of the matrix, ordered by rows.
             */
            TransformMatrix_t(std::array<double, 16> values);

            std::array<double, 16> matrix_; /**< @brief Array of the 16 values in the 4x4 matrix.*/
            std::array<double, 16> matrix_inverse_; /**< @brief Transposed inverted matrix, used to transform directions.*/

            /**
             * @brief Rotates the matrix around the x axis by a specified angle in radians.
             *
             * This means that if an object it at (0, 1, 0) and is rotated π rad, it will be at (0, -1, 0) and face the opposite direction.
             *
             * @param angle Angle in radians to rotate around the x axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateXAxis(double angle) -> TransformMatrix_t&;

            /**
             * @brief Rotates the matrix around the x axis by a specified angle in radians.
             *
             * This means that if an object it at (1, 0, 0) and is rotated π rad, it will be at (-1, 0, 0) and face the opposite direction.
             *
             * @param angle Angle in radians to rotate around the y axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateYAxis(double angle) -> TransformMatrix_t&;

            /**
             * @brief Rotates the matrix around the z axis by a specified angle in radians.
             *
             * This means that if an object it at (1, 0, 0) and is rotated π rad, it will be at (-1, 0, 0) and face the opposite direction.
             *
             * @param angle Angle in radians to rotate around the z axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateZAxis(double angle) -> TransformMatrix_t&;

            /**
             * @brief Rotates the matrix on itself around the x direction.
             *
             * The object won't move but will face the opposite direction around x.
             *
             * @param angle Angle in radians to rotate on itself around the x direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateX(double angle) -> TransformMatrix_t&;

            /**
             * @brief Rotates the matrix on itself around the y direction.
             *
             * The object won't move but will face the opposite direction around y.
             *
             * @param angle Angle in radians to rotate on itself around the y direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateY(double angle) -> TransformMatrix_t&;

            /**
             * @brief Rotates the matrix on itself around the z direction.
             *
             * The object won't move but will face the opposite direction around z.
             *
             * @param angle Angle in radians to rotate on itself around the z direction.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto rotateZ(double angle) -> TransformMatrix_t&;

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
            auto rotateAxis(const Vec3f& vec, double angle) -> TransformMatrix_t&;

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
            auto rotate(const Vec3f& vec, double angle) -> TransformMatrix_t&;

            /**
             * @brief Translates the matrix in 3D space.
             *
             * @param vec Vector by which to translate the matrix.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto translate(const Vec3f& vec) -> TransformMatrix_t&;

            /**
             * @brief Scales the matrix from (0, 0, 0) by a three-component vector.
             *
             * This means that if an object is at (1, 0, 1) and is scaled by (2, 2, 1) it will
             * be placed at (2, 0, 1) and be twice bigger except for the z direction.
             *
             * @param vec Vector whose three components are the scale factors for the three axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto scaleAxis(const Vec3f& vec) -> TransformMatrix_t&;

            /**
             * @brief Scales the matrix from (0, 0, 0) by a factor.
             *
             * This means that if an object is at (1, 0, 1) and is scaled by 2 it will
             * be placed at (2, 0, 2) and be twice bigger.
             *
             * @param fac Factor by which the matrix will be scaled from (0, 0, 0).
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto scaleAxis(double fac) -> TransformMatrix_t&;

            /**
             * @brief Scales the matrix inplace by a three-component vector.
             *
             * This won't move the object.
             *
             * @param vec Vector whose three components are the scale factors for the three axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto scale(const Vec3f& vec) -> TransformMatrix_t&;

            /**
             * @brief Scales the matrix inplace by a factor.
             *
             * This won't move the object.
             *
             * @param fac Scale factors for the three axis.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto scale(double fac) -> TransformMatrix_t&;

            /**
             * @brief Reflects the matrix. Not implemented yet.
             *
             * Not implemented.
             *
             * @param vec Vector around which to reflect.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto reflect(const Vec3f& vec) -> TransformMatrix_t&;

            /**
             * @brief Shears the matrix. Not implemented yet.
             *
             * Not implemented.
             *
             * @param vec Vector along which to shear.
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto shear(const Vec3f& vec) -> TransformMatrix_t&;

            /**
             * @brief Transposes the matrix.
             *
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto transpose() -> TransformMatrix_t&;

            /**
             * @brief Inverts the matrix.
             *
             * See https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html
             *
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto invert() -> TransformMatrix_t&;

            /**
             * @brief Negates the matrix.
             *
             * @return TransformMatrix_t& Reference to this matrix, used to chain operations.
             */
            auto neg() -> TransformMatrix_t&;

            /**
             * @brief Transforms a point with the matrix, moving it around according to the operations made on the matrix.
             *
             * @param vec Point to transform.
             * @return Vec3f Transformed point.
             */
            auto multVec(const Vec3f& vec) const -> Vec3f;

            /**
             * @brief Transforms a direction, rotating it according to the operations made on the matrix.
             *
             * @param vec Direction to transform.
             * @return Vec3f Transformed direction.
             */
            auto multDir(const Vec3f& vec) const -> Vec3f;

            /**
             * @brief Get the maximum scale of all three axis.
             *
             * @return double Maximum scale of the matrix.
             */
            auto getScale() const -> double;

        private:
            /**
             * @brief Builds the transposed inverse matrix, to transform directions.
             *
             * This should be called after each transformation to ensure the inverse is up to date.
             */
            auto buildInverse() -> void;
    };
}}

#endif