#include "entities/Vec3f.h"
#include <algorithm>
#include <cmath>

using APTracer::Entities::Vec3f;

Vec3f::Vec3f() : v{0, 0, 0} {
} 
Vec3f::Vec3f(double x, double y, double z) : v{x, y, z} {
} 
Vec3f::Vec3f(double x) : v{x, x, x} {
} 
auto Vec3f::operator[](int index) -> double& {
    return v[index];
} 
auto Vec3f::operator[](int index) const -> double {
    return v[index];
} 
auto Vec3f::operator[](unsigned int index) -> double& {
    return v[index];
} 
auto Vec3f::operator[](unsigned int index) const -> double {
    return v[index];
} 
auto Vec3f::operator*(double scale) const -> Vec3f {
    return {v[0] * scale, v[1] * scale, v[2] * scale};
} 
auto Vec3f::operator*(const Vec3f &other) const -> Vec3f {
    return {v[0] * other[0], v[1] * other[1], v[2] * other[2]};
} 
auto Vec3f::operator/(double scale) const -> Vec3f {
    return {v[0] / scale, v[1] / scale, v[2] / scale};
} 
auto Vec3f::operator/(const Vec3f &other) const -> Vec3f {
    return {v[0] / other[0], v[1] / other[1], v[2] / other[2]};
}
auto Vec3f::operator+(const Vec3f &other) const -> Vec3f {
    return {v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]};
} 
auto Vec3f::operator+(double factor) const -> Vec3f {
    return {v[0] + factor, v[1] + factor, v[2] + factor};
} 
auto Vec3f::operator-(const Vec3f &other) const -> Vec3f {
    return {v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]};
} 
auto Vec3f::operator-(double factor) const -> Vec3f {
    return {v[0] - factor, v[1] - factor, v[2] - factor};
}
auto Vec3f::operator-() const -> Vec3f {
    return {-v[0], -v[1], -v[2]};
} 
auto Vec3f::operator==(const Vec3f &other) const -> bool {
    return (v[0] == other.v[0]) && (v[1] == other.v[1]) && (v[2] == other.v[2]);
}
auto Vec3f::operator*=(double scale) -> const Vec3f& {
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    return *this;
} 
auto Vec3f::operator*=(const Vec3f &other) -> const Vec3f& {
    v[0] *= other[0];
    v[1] *= other[1];
    v[2] *= other[2];
    return *this;
}
auto Vec3f::operator/=(double scale) -> const Vec3f& {
    v[0] /= scale;
    v[1] /= scale;
    v[2] /= scale;
    return *this;
} 
auto Vec3f::operator/=(const Vec3f &other) -> const Vec3f& {
    v[0] /= other[0];
    v[1] /= other[1];
    v[2] /= other[2];
    return *this;
}
auto Vec3f::operator+=(const Vec3f &other) -> const Vec3f& {
    v[0] += other.v[0];
    v[1] += other.v[1];
    v[2] += other.v[2];
    return *this;
} 
auto Vec3f::operator+=(double factor) -> const Vec3f& {
    v[0] += factor;
    v[1] += factor;
    v[2] += factor;
    return *this;
} 
auto Vec3f::operator-=(const Vec3f &other) -> const Vec3f& {
    v[0] -= other.v[0];
    v[1] -= other.v[1];
    v[2] -= other.v[2];
    return *this;
} 
auto Vec3f::operator-=(double factor) -> const Vec3f& {
    v[0] -= factor;
    v[1] -= factor;
    v[2] -= factor;
    return *this;
}
auto Vec3f::min(const Vec3f &other) -> Vec3f& {
    v[0] = std::min(v[0], other[0]);
    v[1] = std::min(v[1], other[1]);
    v[2] = std::min(v[2], other[2]);
    return *this;
}
auto Vec3f::min(double other) -> Vec3f& {
    v[0] = std::min(v[0], other);
    v[1] = std::min(v[1], other);
    v[2] = std::min(v[2], other);
    return *this;
}
auto Vec3f::max(const Vec3f &other) -> Vec3f& {
    v[0] = std::max(v[0], other[0]);
    v[1] = std::max(v[1], other[1]);
    v[2] = std::max(v[2], other[2]);
    return *this;
}
auto Vec3f::max(double other) -> Vec3f& {
    v[0] = std::max(v[0], other);
    v[1] = std::max(v[1], other);
    v[2] = std::max(v[2], other);
    return *this;
}
auto Vec3f::getMin(const Vec3f &other) const -> Vec3f {
    return {std::min(v[0], other[0]), std::min(v[1], other[1]), std::min(v[2], other[2])};
}
auto Vec3f::getMin(double other) const -> Vec3f {
    return {std::min(v[0], other), std::min(v[1], other), std::min(v[2], other)};
}
auto Vec3f::getMax(const Vec3f &other) const -> Vec3f {
    return {std::max(v[0], other[0]), std::max(v[1], other[1]), std::max(v[2], other[2])};
}
auto Vec3f::getMax(double other) const -> Vec3f {
    return {std::max(v[0], other), std::max(v[1], other), std::max(v[2], other)};
}
auto Vec3f::magnitude() const -> double {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
} 
auto Vec3f::magnitudeSquared() const -> double {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
} 
auto Vec3f::normalize() const -> Vec3f {
    const double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return {v[0] / m, v[1] / m, v[2] / m};
} 
auto Vec3f::normalize_inplace() -> const Vec3f& {
    const double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= m;
    v[1] /= m;
    v[2] /= m;
    return *this;
}
auto Vec3f::dot(const Vec3f &other) const -> double {
    return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
} 
auto Vec3f::cross(const Vec3f &other) const -> Vec3f {
    return {v[1] * other.v[2] - v[2] * other.v[1],
            v[2] * other.v[0] - v[0] * other.v[2],
            v[0] * other.v[1] - v[1] * other.v[0]};
} 
auto Vec3f::to_sph() -> const Vec3f& { // CHECK outputs nan
    // [r, theta, phi] (theta is polar angle)
    const double temp = std::atan2(v[1], v[0]);
    v[0] = magnitude();
    v[1] = std::acos(v[2]/v[0]);
    v[1] = std::isnan(v[1]) ? 0.0 : v[1];
    v[2] = temp;
    return *this;
}
auto Vec3f::to_xyz() -> const Vec3f& {
    const double temp = v[2];
    v[2] = v[0]*std::cos(v[1]);
    const double temp2 = v[1];
    v[1] = v[0]*std::sin(temp2)*std::sin(temp);
    v[0] *= std::sin(temp2)*std::cos(temp);
    return *this;
}
auto Vec3f::to_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) -> const Vec3f& {
    const Vec3f temp = Vec3f(v[0]*std::sin(v[1])*std::cos(v[2]), v[0]*std::sin(v[1])*std::sin(v[2]),  v[0]*std::cos(v[1])); // CHECK could be better
    v[0] = ref1[0] * temp[0] + ref2[0] * temp[1] + ref3[0] * temp[2];
    v[1] = ref1[1] * temp[0] + ref2[1] * temp[1] + ref3[1] * temp[2];
    v[2] = ref1[2] * temp[0] + ref2[2] * temp[1] + ref3[2] * temp[2];
    return *this;
}
auto Vec3f::get_sph() const -> Vec3f {
    const double r = magnitude();
    const double temp = std::acos(v[2]/r);
    return {r, std::isnan(temp) ? 0.0 : temp, std::atan2(v[1], v[0])};
}
auto Vec3f::get_xyz() const -> Vec3f {
    return {v[0]*std::sin(v[1])*std::cos(v[2]), v[0]*std::sin(v[1])*std::sin(v[2]), v[0]*std::cos(v[1])};
}
auto Vec3f::get_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) const -> Vec3f {
    return ref1 * v[0]*std::sin(v[1])*std::cos(v[2]) + ref2 * v[0]*std::sin(v[1])*std::sin(v[2]) + ref3 * v[0]*std::cos(v[1]);
}
auto Vec3f::ln() const -> Vec3f {
    return {std::log(v[0]), std::log(v[1]), std::log(v[2])};
}
auto Vec3f::sqrt() const -> Vec3f {
    return {std::sqrt(v[0]), std::sqrt(v[1]), std::sqrt(v[2])};
}
auto Vec3f::exp() const -> Vec3f {
    return {std::exp(v[0]), std::exp(v[1]), std::exp(v[2])};
}
auto Vec3f::pow(double exp) const -> Vec3f {
    return {std::pow(v[0], exp), std::pow(v[1], exp), std::pow(v[2], exp)};
}
auto Vec3f::pow_inplace(double exp) -> Vec3f& {
    v[0] = std::pow(v[0], exp);
    v[1] = std::pow(v[1], exp);
    v[2] = std::pow(v[2], exp);
    return *this;
}
auto Vec3f::floor() const -> Vec3f {
    return {std::floor(v[0]), std::floor(v[1]), std::floor(v[2])};
}
auto Vec3f::ceil() const -> Vec3f {
    return {std::ceil(v[0]), std::ceil(v[1]), std::ceil(v[2])};
}
auto Vec3f::round_inplace() -> Vec3f& {
    v[0] = std::round(v[0]);
    v[1] = std::round(v[1]);
    v[2] = std::round(v[2]);
    return *this;
}
auto Vec3f::clamp(double minimum, double maximum) -> Vec3f& {
    min(maximum);
    max(minimum);
    return *this;
}
auto operator<<(std::ostream &output, const Vec3f &v) -> std::ostream& {
    std::cout << '[' << v[0] << ", " << v[1] << ", " << v[2] << ']';
    return output;
} 
auto operator*(const double factor, const Vec3f &v) -> Vec3f {
    return {v[0] * factor, v[1] * factor, v[2] * factor};
}
auto Vec3f::x() const -> double {
    return v[0];
}
auto Vec3f::y() const -> double {
    return v[1];
}
auto Vec3f::z() const -> double {
    return v[2];
}
auto Vec3f::x() -> double& {
    return v[0];
}
auto Vec3f::y() -> double& {
    return v[1];
}
auto Vec3f::z() -> double& {
    return v[2];
}
auto Vec3f::r() const -> double {
    return v[0];
}
auto Vec3f::g() const -> double {
    return v[1];
}
auto Vec3f::b() const -> double {
    return v[2];
}
auto Vec3f::r() -> double& {
    return v[0];
}
auto Vec3f::g() -> double& {
    return v[1];
}
auto Vec3f::b() -> double& {
    return v[2];
}