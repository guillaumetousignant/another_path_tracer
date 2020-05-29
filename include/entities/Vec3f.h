#ifndef VEC3F_H
#define VEC3F_H

#include <iostream>

namespace APTracer { namespace Entities {

    /**
     * @brief The Vec3f class represents a 3-element vector.
     * 
     * This can be used for 3D coordinates, 3D directions, 3-component colours, etc.
     * Several arithmetical operations are defined for those vectors.
     */
    class Vec3f {
        public:
            double v[3]; /**< @brief Array of the 3 values in vector.*/
        public:
            /**
             * @brief Construct a new Vec3f object with (0, 0, 0).
             */
            Vec3f();

            /**
             * @brief Construct a new Vec3f object from 3 components.
             * 
             * @param x First component of the vector.
             * @param y Second component of the vector.
             * @param z Third component of the vector.
             */
            Vec3f(double x, double y, double z); 

            /**
             * @brief Construct a new Vec 3f object from one value.
             * 
             * @param x Value given to the three components of the vector.
             */
            Vec3f(double x); 

            /**
             * @brief Accesses the selected component of the vector, returning a reference.
             * 
             * @param index Index of the component to access.
             * @return double& Reference to the selected component.
             */
            double &operator[](int index);

            /**
             * @brief Returns the selected component of the vector.
             * 
             * @param index Index of the component to return.
             * @return double Selected component.
             */
            double operator[](int index) const; 

            /**
             * @brief Accesses the selected component of the vector, returning a reference.
             * 
             * @param index Index of the component to access.
             * @return double& Reference to the selected component.
             */
            double &operator[](unsigned int index);

            /**
             * @brief Returns the selected component of the vector.
             * 
             * @param index Index of the component to return.
             * @return double Selected component.
             */
            double operator[](unsigned int index) const; 

            /**
             * @brief Multiplies all components of the vector by a factor.
             * 
             * Returns (x1*a, y1*a, z1*a).
             * 
             * @param scale Factor used to multiply all components of the vector.
             * @return Vec3f Resulting vector, (x1*a, y1*a, z1*a).
             */
            Vec3f operator*(double scale) const;

            /**
             * @brief Element-wise multiplication of two vectors.
             * 
             * Returns (x1*x2, y1*y2, z1*z3).
             * 
             * @param other Vector used to multiply.
             * @return Vec3f Resulting vector, (x1*x2, y1*y2, z1*z3).
             */
            Vec3f operator*(const Vec3f &other) const;

            /**
             * @brief Divides all components of the vector by a factor.
             * 
             * Returns (x1/a, y1/a, z1/a).
             * 
             * @param scale Factor used to divide all components of the vector.
             * @return Vec3f Resulting vector, (x1/a, y1/a, z1/a).
             */
            Vec3f operator/(double scale) const;

            /**
             * @brief Elements-wise division by the provided vector.
             * 
             * Returns (x1/x2, y1/y2, z1/z2).
             * 
             * @param other Vector used to divide the components of this vector.
             * @return Vec3f Resulting vector, (x1/x2, y1/y2, z1/z2).
             */
            Vec3f operator/(const Vec3f &other) const;

            /**
             * @brief Adds two vectors.
             * 
             * Returns (x1+x2, y1+y2, z1+z2).
             * 
             * @param other Vector added to this vector.
             * @return Vec3f Resulting vector, (x1+x2, y1+y2, z1+z2).
             */
            Vec3f operator+(const Vec3f &other) const;

            /**
             * @brief Adds a factor to all components of the vector.
             * 
             * Returns (x1+a, y1+a, z1+a).
             * 
             * @param factor Factor added to all components of the vector.
             * @return Vec3f Resulting vector, (x1+a, y1+a, z1+a).
             */
            Vec3f operator+(double factor) const;

            /**
             * @brief Substracts a vector from this vector.
             * 
             * Returns (x1-x2, y1-y2, z1-z2).
             * 
             * @param other Vector to substract from this vector.
             * @return Vec3f Resulting vector, (x1-x2, y1-y2, z1-z2).
             */
            Vec3f operator-(const Vec3f &other) const;

            /**
             * @brief Substracts a factor from all components of the vector.
             * 
             * Returns (x1-a, y1-a, z1-a).
             * 
             * @param factor Factor substracted from all components of the vector.
             * @return Vec3f Resulting vector, (x1-a, y1-a, z1-a).
             */
            Vec3f operator-(double factor) const;

            /**
             * @brief Returns the vector negated.
             * 
             * Returns (-x1, -y1, -z1).
             * 
             * @return Vec3f Resulting vector, (-x1, -y1, -z1).
             */
            Vec3f operator-() const; 

            /**
             * @brief Tests equality between two vectors.
             * 
             * @param other Vector used to test equality.
             * @return true All three components of the vectors are equal.
             * @return false At least one component of the vectors is different.
             */
            bool operator==(const Vec3f &other) const;

            /**
             * @brief In-place multiplies all components of the vector by a factor.
             * 
             * Becomes (x1*a, y1*a, z1*a).
             * 
             * @param scale Factor used to multiply all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator*=(double scale);

            /**
             * @brief In-place element-wise multiplication of the vector by another vector.
             * 
             * Becomes (x1*x2, y1*y2, z1*z2).
             * 
             * @param other Vector used to multiply.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator*=(const Vec3f &other);

            /**
             * @brief In-place divides all components of the vector by a factor.
             * 
             * Becomes (x1/a, y1/a, z1/a).
             * 
             * @param scale Factor used to divide all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator/=(double scale);

            /**
             * @brief In-place elements-wise division by the provided vector.
             * 
             * Becomes (x1/x2, y1/y2, z1/z2).
             * 
             * @param other Vector used to divide the components of this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator/=(const Vec3f &other);

            /**
             * @brief In-place addition of another vector.
             * 
             * Becomes (x1+x2, y1+y2, z1+z2).
             * 
             * @param other Vector added to this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator+=(const Vec3f &other);

            /**
             * @brief In-place adds a factor to all components of the vector.
             * 
             * Becomes (x1+a, y1+a, z1+a).
             * 
             * @param factor Factor added to all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator+=(double factor);

            /**
             * @brief In-place substracts a vector from this vector.
             * 
             * Becomes (x1-x2, y1-y2, z1-z2).
             * 
             * @param other Vector to substract from this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator-=(const Vec3f &other); 

            /**
             * @brief In-place substracts a factor from all components of the vector.
             * 
             * Becomes (x1-a, y1-a, z1-a).
             * 
             * @param factor Factor substracted from all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &operator-=(double factor);

            /**
             * @brief Sets the components of the vector to the minimum of its components and the other's.
             * 
             * Becomes (min(x1, x2), min(y1, y2), min(z1, z2))
             * 
             * @param other Vector to calculate minimum components with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &min(const Vec3f &other);

            /**
             * @brief Sets the components of the vector to the minimum of its components and the provided factor.
             * 
             * Becomes (min(x1, a), min(y1, a), min(z1, a))
             * 
             * @param other Factor to calculate minimum with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &min(double other);

            /**
             * @brief ets the components of the vector to the maximum of its components and the other's.
             * 
             * Becomes (max(x1, x2), max(y1, y2), max(z1, z2))
             * 
             * @param other Vector to calculate maximum components with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &max(const Vec3f &other);

            /**
             * @brief Sets the components of the vector to the maximum of its components and the provided factor.
             * 
             * Becomes (max(x1, a), max(y1, a), max(z1, a))
             * 
             * @param other Factor to calculate maximum with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &max(double other);

            /**
             * @brief Returns a vector with the minimum components of this vector and another.
             * 
             * Returns (min(x1, x2), min(y1, y2), min(z1, z2))
             * 
             * @param other Vector to calculate minimum components with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            Vec3f getMin(const Vec3f &other) const;

            /**
             * @brief Returns a vector with the minimum components of this vector and a factor.
             * 
             * Returns (min(x1, a), min(y1, a), min(z1, a))
             * 
             * @param other Factor to calculate minimum with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            Vec3f getMin(double other) const;

            /**
             * @brief Returns a vector with the maximum components of this vector and another.
             * 
             * Returns (max(x1, x2), max(y1, y2), max(z1, z2))
             * 
             * @param other Vector to calculate maximum components with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            Vec3f getMax(const Vec3f &other) const;

            /**
             * @brief Returns a vector with the maximum components of this vector and a factor.
             * 
             * Returns (max(x1, a), max(y1, a), max(z1, a))
             * 
             * @param other Factor to calculate maximum with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            Vec3f getMax(double other) const;

            /**
             * @brief Returns the magnitude of the vector.
             * 
             * Returns the L2 norm of the vector: sqrt(x^2 + 1^2 + z^2).
             * 
             * @return double Magnitude of the vector.
             */
            double magnitude() const;

            /**
             * @brief Returns the squared magnitude of the vector.
             * 
             * Returns x^2 + 1^2 + z^2. Useful because it is much faster than the norm,
             * and can be used instead of it in some situations.
             * 
             * @return double Squared magnitude of the norm.
             */
            double magnitudeSquared() const;
            Vec3f normalize() const;
            const Vec3f &normalize_inplace();
            double dot(const Vec3f &other) const;
            Vec3f cross(const Vec3f &other) const;
            const Vec3f &to_sph();
            const Vec3f &to_xyz();
            const Vec3f &to_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3);
            Vec3f get_sph() const;
            Vec3f get_xyz() const;
            Vec3f get_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) const;
            Vec3f ln() const;
            Vec3f sqrt() const;
            Vec3f exp() const;
            Vec3f pow(double exp) const;
            Vec3f &pow_inplace(double exp);
            Vec3f floor() const;
            Vec3f ceil() const;
            Vec3f &round_inplace();
            Vec3f &clamp(double minimum, double maximum);
    };
}}

std::ostream &operator<<(std::ostream &output, const APTracer::Entities::Vec3f &v);

#endif