#ifndef APTRACER_VEC3F_H
#define APTRACER_VEC3F_H

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

            /**
             * @brief Returns a the normalized vector.
             * 
             * Divides all components of the vector by its magnitude.
             * 
             * @return Vec3f Normalized vector.
             */
            Vec3f normalize() const;

            /**
             * @brief Normalizes the vector in-place, dividing it by its norm.
             * 
             * Divides all components of the vector by its magnitude.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &normalize_inplace();

            /**
             * @brief Computes the dot product of this vector and another.
             * 
             * Returns v1.v2
             * 
             * @param other Vector to dot with this one.
             * @return double Dot product of the two vectors.
             */
            double dot(const Vec3f &other) const;

            /**
             * @brief Computes the cross product of this vector and another.
             * 
             * Returns v1 x v2.
             * 
             * @param other Vector to cross with this one.
             * @return Vec3f Cross product of the two vectors.
             */
            Vec3f cross(const Vec3f &other) const;

            /**
             * @brief Changes the vector in-place to spherical coordinates.
             * 
             * Assumes the vector is in cartesian coordinates.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &to_sph();

            /**
             * @brief Changes the vector in-place to cartesian coordinates.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &to_xyz();

            /**
             * @brief Changes the vector in-place to cartesian coordinates with arbitrary axises.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @param ref1 Axis used for x.
             * @param ref2 Axis used for y.
             * @param ref3 Axis used for z.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            const Vec3f &to_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3);

            /**
             * @brief Returns the vector in spherical coordinates.
             * 
             * Assumes the vector is in cartesian coordinates.
             * 
             * @return Vec3f Spherical coordinates of the vector.
             */
            Vec3f get_sph() const;

            /**
             * @brief Returns the vector in cartesian coordinates.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @return Vec3f Cartesian coordinates of the vector.
             */
            Vec3f get_xyz() const;

            /**
             * @brief Returns the vector in cartesian coordinates with arbitrary axises.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @param ref1 Axis used for x.
             * @param ref2 Axis used for y.
             * @param ref3 Axis used for z.
             * @return Vec3f Cartesian coordinates of the vector.
             */
            Vec3f get_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) const;

            /**
             * @brief Returns a vector of the natural logarithm of all components of the vector.
             * 
             * Returns (ln(x), ln(y), ln(z))
             * 
             * @return Vec3f Vector made of the natural logarithm of all components of the vector.
             */
            Vec3f ln() const;

            /**
             * @brief Returns a vector of the square root of all components of the vector.
             * 
             * Returns (sqrt(x), sqrt(y), sqrt(z))
             * 
             * @return Vec3f Vector made of the square root of all components of the vector.
             */
            Vec3f sqrt() const;

            /**
             * @brief Returns a vector of the exponential of all components of the vector.
             * 
             * Returns (e^x, e^y, e^z).
             * 
             * @return Vec3f Vector made of the exponential of all components of the vector.
             */
            Vec3f exp() const;

            /**
             * @brief Returns a vector of the components of the vector to the specified power.
             * 
             * Returns (x^a, y^a, z^a).
             * 
             * @param exp Power to be applied to all components.
             * @return Vec3f Vector made of the components of the vector to the specified power.
             */
            Vec3f pow(double exp) const;

            /**
             * @brief In-place raise the components of the vector to the specified power.
             * 
             * Becomes (x^a, y^a, z^a).
             * 
             * @param exp Power to be applied to all components.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &pow_inplace(double exp);

            /**
             * @brief Returns a vector of the components of the vector rounded down.
             * 
             * Returns (floor(x), floor(y), floor(z))
             * 
             * @return Vec3f Vector made of the components of the vector rounded down.
             */
            Vec3f floor() const;

            /**
             * @brief Returns a vector of the components of the vector rounded up.
             * 
             * Returns (ceil(x), ceil(y), ceil(z))
             * 
             * @return Vec3f Vector made of the components of the vector rounded up.
             */
            Vec3f ceil() const;

            /**
             * @brief In-place rounds the components to the nearest integer value.
             * 
             * Becomes (round(x), round(y), round(z))
             * 
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &round_inplace();

            /**
             * @brief In-place limits the components of the vector to a minimum and maximum value.
             * 
             * @param minimum Minimum value of the components.
             * @param maximum Maximum value of the components.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            Vec3f &clamp(double minimum, double maximum);
    };
}}

/**
 * @brief Formats a vector to be displayed.
 * 
 * @param output Output stream.
 * @param v Vector to be displayed.
 * @return std::ostream& Output stream.
 */
std::ostream &operator<<(std::ostream &output, const APTracer::Entities::Vec3f &v);

/**
 * @brief Multiplies a factor with a vector.
 * 
 * Returns (a*x, a*y, a*z).
 * 
 * @param factor Factor multiplying the vector.
 * @param v Vector to be multiplied.
 * @return APTracer::Entities::Vec3f Resulting Vector, (a*x, a*y, a*z).
 */
APTracer::Entities::Vec3f operator*(const double factor, const APTracer::Entities::Vec3f &v);

#endif