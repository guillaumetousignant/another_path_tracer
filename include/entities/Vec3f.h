#ifndef APTRACER_ENTITIES_VEC3F_H
#define APTRACER_ENTITIES_VEC3F_H

#include <iostream>
#include <array>

namespace APTracer { namespace Entities {

    /**
     * @brief The Vec3f class represents a 3-element vector.
     * 
     * This can be used for 3D coordinates, 3D directions, 3-component colours, etc.
     * Several arithmetical operations are defined for those vectors.
     */
    class Vec3f {
        public:
            std::array<double, 3> v; /**< @brief Array of the 3 values in the vector.*/
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
             * @brief Construct a new Vec3f object from an array of values.
             * 
             * @param values Array containing the three values of the vector.
             */
            Vec3f(std::array<double, 3> values);

            /**
             * @brief Construct a new Vec3f object from one value.
             * 
             * @param x Value given to the three components of the vector.
             */
            explicit Vec3f(double x);

            /**
             * @brief Accesses the selected component of the vector, returning a reference.
             * 
             * @param index Index of the component to access.
             * @return double& Reference to the selected component.
             */
            auto operator[](int index) -> double&;

            /**
             * @brief Returns the selected component of the vector.
             * 
             * @param index Index of the component to return.
             * @return double Selected component.
             */
            auto operator[](int index) const -> double; 

            /**
             * @brief Accesses the selected component of the vector, returning a reference.
             * 
             * @param index Index of the component to access.
             * @return double& Reference to the selected component.
             */
            auto operator[](unsigned int index) -> double&;

            /**
             * @brief Returns the selected component of the vector.
             * 
             * @param index Index of the component to return.
             * @return double Selected component.
             */
            auto operator[](unsigned int index) const -> double; 

            /**
             * @brief Multiplies all components of the vector by a factor.
             * 
             * Returns (x1*a, y1*a, z1*a).
             * 
             * @param scale Factor used to multiply all components of the vector.
             * @return Vec3f Resulting vector, (x1*a, y1*a, z1*a).
             */
            auto operator*(double scale) const -> Vec3f;

            /**
             * @brief Element-wise multiplication of two vectors.
             * 
             * Returns (x1*x2, y1*y2, z1*z3).
             * 
             * @param other Vector used to multiply.
             * @return Vec3f Resulting vector, (x1*x2, y1*y2, z1*z3).
             */
            auto operator*(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Divides all components of the vector by a factor.
             * 
             * Returns (x1/a, y1/a, z1/a).
             * 
             * @param scale Factor used to divide all components of the vector.
             * @return Vec3f Resulting vector, (x1/a, y1/a, z1/a).
             */
            auto operator/(double scale) const -> Vec3f;

            /**
             * @brief Elements-wise division by the provided vector.
             * 
             * Returns (x1/x2, y1/y2, z1/z2).
             * 
             * @param other Vector used to divide the components of this vector.
             * @return Vec3f Resulting vector, (x1/x2, y1/y2, z1/z2).
             */
            auto operator/(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Adds two vectors.
             * 
             * Returns (x1+x2, y1+y2, z1+z2).
             * 
             * @param other Vector added to this vector.
             * @return Vec3f Resulting vector, (x1+x2, y1+y2, z1+z2).
             */
            auto operator+(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Adds a factor to all components of the vector.
             * 
             * Returns (x1+a, y1+a, z1+a).
             * 
             * @param factor Factor added to all components of the vector.
             * @return Vec3f Resulting vector, (x1+a, y1+a, z1+a).
             */
            auto operator+(double factor) const -> Vec3f;

            /**
             * @brief Substracts a vector from this vector.
             * 
             * Returns (x1-x2, y1-y2, z1-z2).
             * 
             * @param other Vector to substract from this vector.
             * @return Vec3f Resulting vector, (x1-x2, y1-y2, z1-z2).
             */
            auto operator-(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Substracts a factor from all components of the vector.
             * 
             * Returns (x1-a, y1-a, z1-a).
             * 
             * @param factor Factor substracted from all components of the vector.
             * @return Vec3f Resulting vector, (x1-a, y1-a, z1-a).
             */
            auto operator-(double factor) const -> Vec3f;

            /**
             * @brief Returns the vector negated.
             * 
             * Returns (-x1, -y1, -z1).
             * 
             * @return Vec3f Resulting vector, (-x1, -y1, -z1).
             */
            auto operator-() const -> Vec3f; 

            /**
             * @brief Tests equality between two vectors.
             * 
             * @param other Vector used to test equality.
             * @return true All three components of the vectors are equal.
             * @return false At least one component of the vectors is different.
             */
            auto operator==(const Vec3f &other) const -> bool;

            /**
             * @brief In-place multiplies all components of the vector by a factor.
             * 
             * Becomes (x1*a, y1*a, z1*a).
             * 
             * @param scale Factor used to multiply all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator*=(double scale) -> const Vec3f&;

            /**
             * @brief In-place element-wise multiplication of the vector by another vector.
             * 
             * Becomes (x1*x2, y1*y2, z1*z2).
             * 
             * @param other Vector used to multiply.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator*=(const Vec3f &other) -> const Vec3f&;

            /**
             * @brief In-place divides all components of the vector by a factor.
             * 
             * Becomes (x1/a, y1/a, z1/a).
             * 
             * @param scale Factor used to divide all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator/=(double scale) -> const Vec3f&;

            /**
             * @brief In-place elements-wise division by the provided vector.
             * 
             * Becomes (x1/x2, y1/y2, z1/z2).
             * 
             * @param other Vector used to divide the components of this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator/=(const Vec3f &other) -> const Vec3f&;

            /**
             * @brief In-place addition of another vector.
             * 
             * Becomes (x1+x2, y1+y2, z1+z2).
             * 
             * @param other Vector added to this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator+=(const Vec3f &other) -> const Vec3f&;

            /**
             * @brief In-place adds a factor to all components of the vector.
             * 
             * Becomes (x1+a, y1+a, z1+a).
             * 
             * @param factor Factor added to all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator+=(double factor) -> const Vec3f&;

            /**
             * @brief In-place substracts a vector from this vector.
             * 
             * Becomes (x1-x2, y1-y2, z1-z2).
             * 
             * @param other Vector to substract from this vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator-=(const Vec3f &other) -> const Vec3f&; 

            /**
             * @brief In-place substracts a factor from all components of the vector.
             * 
             * Becomes (x1-a, y1-a, z1-a).
             * 
             * @param factor Factor substracted from all components of the vector.
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto operator-=(double factor) -> const Vec3f&;

            /**
             * @brief Sets the components of the vector to the minimum of its components and the other's.
             * 
             * Becomes (min(x1, x2), min(y1, y2), min(z1, z2))
             * 
             * @param other Vector to calculate minimum components with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto min(const Vec3f &other) -> Vec3f&;

            /**
             * @brief Sets the components of the vector to the minimum of its components and the provided factor.
             * 
             * Becomes (min(x1, a), min(y1, a), min(z1, a))
             * 
             * @param other Factor to calculate minimum with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto min(double other) -> Vec3f&;

            /**
             * @brief ets the components of the vector to the maximum of its components and the other's.
             * 
             * Becomes (max(x1, x2), max(y1, y2), max(z1, z2))
             * 
             * @param other Vector to calculate maximum components with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto max(const Vec3f &other) -> Vec3f&;

            /**
             * @brief Sets the components of the vector to the maximum of its components and the provided factor.
             * 
             * Becomes (max(x1, a), max(y1, a), max(z1, a))
             * 
             * @param other Factor to calculate maximum with.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto max(double other) -> Vec3f&;

            /**
             * @brief Returns a vector with the minimum components of this vector and another.
             * 
             * Returns (min(x1, x2), min(y1, y2), min(z1, z2))
             * 
             * @param other Vector to calculate minimum components with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            auto getMin(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Returns a vector with the minimum components of this vector and a factor.
             * 
             * Returns (min(x1, a), min(y1, a), min(z1, a))
             * 
             * @param other Factor to calculate minimum with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            auto getMin(double other) const -> Vec3f;

            /**
             * @brief Returns a vector with the maximum components of this vector and another.
             * 
             * Returns (max(x1, x2), max(y1, y2), max(z1, z2))
             * 
             * @param other Vector to calculate maximum components with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            auto getMax(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Returns a vector with the maximum components of this vector and a factor.
             * 
             * Returns (max(x1, a), max(y1, a), max(z1, a))
             * 
             * @param other Factor to calculate maximum with.
             * @return Vec3f Reference to the vector, used to chain operations.
             */
            auto getMax(double other) const -> Vec3f;

            /**
             * @brief Returns the magnitude of the vector.
             * 
             * Returns the L2 norm of the vector: sqrt(x^2 + y^2 + z^2).
             * 
             * @return double Magnitude of the vector.
             */
            auto magnitude() const -> double;

            /**
             * @brief Returns the squared magnitude of the vector.
             * 
             * Returns x^2 + y^2 + z^2. Useful because it is much faster than the norm,
             * and can be used instead of it in some situations.
             * 
             * @return double Squared magnitude of the norm.
             */
            auto magnitudeSquared() const -> double;

            /**
             * @brief Returns a the normalized vector.
             * 
             * Divides all components of the vector by its magnitude.
             * 
             * @return Vec3f Normalized vector.
             */
            auto normalize() const -> Vec3f;

            /**
             * @brief Normalizes the vector in-place, dividing it by its norm.
             * 
             * Divides all components of the vector by its magnitude.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto normalize_inplace() -> const Vec3f&;

            /**
             * @brief Computes the dot product of this vector and another.
             * 
             * Returns v1.v2
             * 
             * @param other Vector to dot with this one.
             * @return double Dot product of the two vectors.
             */
            auto dot(const Vec3f &other) const -> double;

            /**
             * @brief Computes the cross product of this vector and another.
             * 
             * Returns v1 x v2.
             * 
             * @param other Vector to cross with this one.
             * @return Vec3f Cross product of the two vectors.
             */
            auto cross(const Vec3f &other) const -> Vec3f;

            /**
             * @brief Changes the vector in-place to spherical coordinates.
             * 
             * Assumes the vector is in cartesian coordinates.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto to_sph() -> const Vec3f&;

            /**
             * @brief Changes the vector in-place to cartesian coordinates.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @return const Vec3f& Reference to the vector, used to chain operations.
             */
            auto to_xyz() -> const Vec3f&;

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
            auto to_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) -> const Vec3f&;

            /**
             * @brief Returns the vector in spherical coordinates.
             * 
             * Assumes the vector is in cartesian coordinates.
             * 
             * @return Vec3f Spherical coordinates of the vector.
             */
            auto get_sph() const -> Vec3f;

            /**
             * @brief Returns the vector in cartesian coordinates.
             * 
             * Assumes the vector is in spherical coordinates.
             * 
             * @return Vec3f Cartesian coordinates of the vector.
             */
            auto get_xyz() const -> Vec3f;

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
            auto get_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) const -> Vec3f;

            /**
             * @brief Returns a vector of the natural logarithm of all components of the vector.
             * 
             * Returns (ln(x), ln(y), ln(z))
             * 
             * @return Vec3f Vector made of the natural logarithm of all components of the vector.
             */
            auto ln() const -> Vec3f;

            /**
             * @brief Returns a vector of the square root of all components of the vector.
             * 
             * Returns (sqrt(x), sqrt(y), sqrt(z))
             * 
             * @return Vec3f Vector made of the square root of all components of the vector.
             */
            auto sqrt() const -> Vec3f;

            /**
             * @brief Returns a vector of the exponential of all components of the vector.
             * 
             * Returns (e^x, e^y, e^z).
             * 
             * @return Vec3f Vector made of the exponential of all components of the vector.
             */
            auto exp() const -> Vec3f;

            /**
             * @brief Returns a vector of the components of the vector to the specified power.
             * 
             * Returns (x^a, y^a, z^a).
             * 
             * @param exp Power to be applied to all components.
             * @return Vec3f Vector made of the components of the vector to the specified power.
             */
            auto pow(double exp) const -> Vec3f;

            /**
             * @brief In-place raise the components of the vector to the specified power.
             * 
             * Becomes (x^a, y^a, z^a).
             * 
             * @param exp Power to be applied to all components.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto pow_inplace(double exp) -> Vec3f&;

            /**
             * @brief Returns a vector of the components of the vector rounded down.
             * 
             * Returns (floor(x), floor(y), floor(z))
             * 
             * @return Vec3f Vector made of the components of the vector rounded down.
             */
            auto floor() const -> Vec3f;

            /**
             * @brief Returns a vector of the components of the vector rounded up.
             * 
             * Returns (ceil(x), ceil(y), ceil(z))
             * 
             * @return Vec3f Vector made of the components of the vector rounded up.
             */
            auto ceil() const -> Vec3f;

            /**
             * @brief In-place rounds the components to the nearest integer value.
             * 
             * Becomes (round(x), round(y), round(z))
             * 
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto round_inplace() -> Vec3f&;

            /**
             * @brief In-place limits the components of the vector to a minimum and maximum value.
             * 
             * @param minimum Minimum value of the components.
             * @param maximum Maximum value of the components.
             * @return Vec3f& Reference to the vector, used to chain operations.
             */
            auto clamp(double minimum, double maximum) -> Vec3f&;

            /**
             * @brief Returns the x component of the vector
             * 
             * @return double x component of the vector.
             */
            auto x() const -> double;
            
            /**
             * @brief Returns the y component of the vector
             * 
             * @return double y component of the vector.
             */
            auto y() const -> double;

            /**
             * @brief Returns the z component of the vector
             * 
             * @return double z component of the vector.
             */
            auto z() const -> double;

            /**
             * @brief Returns a reference to the x component of the vector
             * 
             * @return double Reference to the x component of the vector.
             */
            auto x() -> double&;
            
            /**
             * @brief Returns a reference to the y component of the vector
             * 
             * @return double Reference to the y component of the vector.
             */
            auto y() -> double&;

            /**
             * @brief Returns a reference to the z component of the vector
             * 
             * @return double Reference to the z component of the vector.
             */
            auto z() -> double&;

            /**
             * @brief Returns the red component of the vector
             * 
             * @return double red component of the vector.
             */
            auto r() const -> double;
            
            /**
             * @brief Returns the green component of the vector
             * 
             * @return double green component of the vector.
             */
            auto g() const -> double;

            /**
             * @brief Returns the blue component of the vector
             * 
             * @return double blue component of the vector.
             */
            auto b() const -> double;

            /**
             * @brief Returns a reference to the red component of the vector
             * 
             * @return double Reference to the red component of the vector.
             */
            auto r() -> double&;
            
            /**
             * @brief Returns a reference to the green component of the vector
             * 
             * @return double Reference to the green component of the vector.
             */
            auto g() -> double&;

            /**
             * @brief Returns a reference to the blue component of the vector
             * 
             * @return double Reference to the blue component of the vector.
             */
            auto b() -> double&;
    };
}}

/**
 * @brief Formats a vector to be displayed.
 * 
 * @param output Output stream.
 * @param v Vector to be displayed.
 * @return std::ostream& Output stream.
 */
auto operator<<(std::ostream &output, const APTracer::Entities::Vec3f &v) -> std::ostream&;

/**
 * @brief Multiplies a factor with a vector.
 * 
 * Returns (a*x, a*y, a*z).
 * 
 * @param factor Factor multiplying the vector.
 * @param v Vector to be multiplied.
 * @return APTracer::Entities::Vec3f Resulting Vector, (a*x, a*y, a*z).
 */
auto operator*(const double factor, const APTracer::Entities::Vec3f &v) -> APTracer::Entities::Vec3f;

#endif