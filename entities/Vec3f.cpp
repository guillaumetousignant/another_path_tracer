#include "Vec3f.h"

Vec3f::Vec3f() {
    v[0] = 0;
    v[1] = 0;
    v[2] = 0; 
} 
Vec3f::Vec3f(double x, double y, double z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
} 
double &Vec3f::operator[](int index) {
    return v[index];
} 
double Vec3f::operator[](int index) const {
    return v[index];
} 
Vec3f Vec3f::operator*(double scale) const {
    return Vec3f(v[0] * scale, v[1] * scale, v[2] * scale);
} 
Vec3f Vec3f::operator*(const Vec3f &other) const {
    return Vec3f(v[0] * other[0], v[1] * other[1], v[2] * other[2]);
} 
Vec3f Vec3f::operator/(double scale) const {
    return Vec3f(v[0] / scale, v[1] / scale, v[2] / scale);
} 
Vec3f Vec3f::operator/(const Vec3f &other) const {
    return Vec3f(v[0] / other[0], v[1] / other[1], v[2] / other[2]);
}
Vec3f Vec3f::operator+(const Vec3f &other)  const{
    return Vec3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
} 
Vec3f Vec3f::operator+(double factor)  const{
    return Vec3f(v[0] + factor, v[1] + factor, v[2] + factor);
} 
Vec3f Vec3f::operator-(const Vec3f &other) const {
    return Vec3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
} 
Vec3f Vec3f::operator-(double factor) const {
    return Vec3f(v[0] - factor, v[1] - factor, v[2] - factor);
}
Vec3f Vec3f::operator-() const {
    return Vec3f(-v[0], -v[1], -v[2]);
} 
bool Vec3f::operator==(const Vec3f &other) const {
    return (v[0] == other.v[0]) && (v[1] == other.v[1]) && (v[2] == other.v[2]);
}
const Vec3f &Vec3f::operator*=(double scale) {
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    return *this;
} 
const Vec3f &Vec3f::operator*=(const Vec3f &other) {
    v[0] *= other[0];
    v[1] *= other[1];
    v[2] *= other[2];
    return *this;
}
const Vec3f &Vec3f::operator/=(double scale) {
    v[0] /= scale;
    v[1] /= scale;
    v[2] /= scale;
    return *this;
} 
const Vec3f &Vec3f::operator/=(const Vec3f &other) {
    v[0] /= other[0];
    v[1] /= other[1];
    v[2] /= other[2];
    return *this;
}
const Vec3f &Vec3f::operator+=(const Vec3f &other) {
    v[0] += other.v[0];
    v[1] += other.v[1];
    v[2] += other.v[2];
    return *this;
} 
const Vec3f &Vec3f::operator+=(double factor) {
    v[0] += factor;
    v[1] += factor;
    v[2] += factor;
    return *this;
} 
const Vec3f &Vec3f::operator-=(const Vec3f &other) {
    v[0] -= other.v[0];
    v[1] -= other.v[1];
    v[2] -= other.v[2];
    return *this;
} 
const Vec3f &Vec3f::operator-=(double factor) {
    v[0] -= factor;
    v[1] -= factor;
    v[2] -= factor;
    return *this;
}
double Vec3f::magnitude() const {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
} 
double Vec3f::magnitudeSquared() const {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
} 
Vec3f Vec3f::normalize() const {
    double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return Vec3f(v[0] / m, v[1] / m, v[2] / m);
} 
double Vec3f::dot(const Vec3f &other) const {
    return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
} 
Vec3f Vec3f::cross(const Vec3f &other) const {
    return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
                 v[2] * other.v[0] - v[0] * other.v[2],
                 v[0] * other.v[1] - v[1] * other.v[0]);
} 
Vec3f Vec3f::ln() const {
    return Vec3f(std::log(v[0]), std::log(v[1]), std::log(v[2]));
}
Vec3f Vec3f::sqrt() const {
    return Vec3f(std::sqrt(v[0]), std::sqrt(v[1]), std::sqrt(v[2]));
}
Vec3f Vec3f::exp() const {
    return Vec3f(std::exp(v[0]), std::exp(v[1]), std::exp(v[2]));
}
ostream &operator<<(ostream &output, const Vec3f &v) {
    cout << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return output;
} 