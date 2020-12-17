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
double &Vec3f::operator[](int index) {
    return v[index];
} 
double Vec3f::operator[](int index) const {
    return v[index];
} 
double &Vec3f::operator[](unsigned int index) {
    return v[index];
} 
double Vec3f::operator[](unsigned int index) const {
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
Vec3f &Vec3f::min(const Vec3f &other){
    v[0] = std::min(v[0], other[0]);
    v[1] = std::min(v[1], other[1]);
    v[2] = std::min(v[2], other[2]);
    return *this;
}
Vec3f &Vec3f::min(double other){
    v[0] = std::min(v[0], other);
    v[1] = std::min(v[1], other);
    v[2] = std::min(v[2], other);
    return *this;
}
Vec3f &Vec3f::max(const Vec3f &other){
    v[0] = std::max(v[0], other[0]);
    v[1] = std::max(v[1], other[1]);
    v[2] = std::max(v[2], other[2]);
    return *this;
}
Vec3f &Vec3f::max(double other){
    v[0] = std::max(v[0], other);
    v[1] = std::max(v[1], other);
    v[2] = std::max(v[2], other);
    return *this;
}
Vec3f Vec3f::getMin(const Vec3f &other) const {
    return Vec3f(std::min(v[0], other[0]), std::min(v[1], other[1]), std::min(v[2], other[2]));
}
Vec3f Vec3f::getMin(double other) const {
    return Vec3f(std::min(v[0], other), std::min(v[1], other), std::min(v[2], other));
}
Vec3f Vec3f::getMax(const Vec3f &other) const{
    return Vec3f(std::max(v[0], other[0]), std::max(v[1], other[1]), std::max(v[2], other[2]));
}
Vec3f Vec3f::getMax(double other) const{
    return Vec3f(std::max(v[0], other), std::max(v[1], other), std::max(v[2], other));
}
double Vec3f::magnitude() const {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
} 
double Vec3f::magnitudeSquared() const {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
} 
Vec3f Vec3f::normalize() const {
    const double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return Vec3f(v[0] / m, v[1] / m, v[2] / m);
} 
const Vec3f &Vec3f::normalize_inplace() {
    const double m = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= m;
    v[1] /= m;
    v[2] /= m;
    return *this;
}
double Vec3f::dot(const Vec3f &other) const {
    return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
} 
Vec3f Vec3f::cross(const Vec3f &other) const {
    return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
                 v[2] * other.v[0] - v[0] * other.v[2],
                 v[0] * other.v[1] - v[1] * other.v[0]);
} 
const Vec3f &Vec3f::to_sph(){ // CHECK outputs nan
    // [r, theta, phi] (theta is polar angle)
    const double temp = std::atan2(v[1], v[0]);
    v[0] = magnitude();
    v[1] = std::acos(v[2]/v[0]);
    v[1] = std::isnan(v[1]) ? 0.0 : v[1];
    v[2] = temp;
    return *this;
}
const Vec3f &Vec3f::to_xyz(){
    const double temp = std::move(v[2]);
    v[2] = v[0]*std::cos(v[1]);
    const double temp2 = std::move(v[1]);
    v[1] = v[0]*std::sin(temp2)*std::sin(temp);
    v[0] *= std::sin(temp2)*std::cos(temp);
    return *this;
}
const Vec3f &Vec3f::to_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3){
    const Vec3f temp = Vec3f(v[0]*std::sin(v[1])*std::cos(v[2]), v[0]*std::sin(v[1])*std::sin(v[2]),  v[0]*std::cos(v[1])); // CHECK could be better
    v[0] = ref1[0] * temp[0] + ref2[0] * temp[1] + ref3[0] * temp[2];
    v[1] = ref1[1] * temp[0] + ref2[1] * temp[1] + ref3[1] * temp[2];
    v[2] = ref1[2] * temp[0] + ref2[2] * temp[1] + ref3[2] * temp[2];
    return *this;
}
Vec3f Vec3f::get_sph() const {
    const double r = magnitude();
    const double temp = std::acos(v[2]/r);
    return Vec3f(r, std::isnan(temp) ? 0.0 : temp, std::atan2(v[1], v[0]));
}
Vec3f Vec3f::get_xyz() const {
    return Vec3f(v[0]*std::sin(v[1])*std::cos(v[2]), v[0]*std::sin(v[1])*std::sin(v[2]), v[0]*std::cos(v[1]));
}
Vec3f Vec3f::get_xyz_offset(const Vec3f &ref1, const Vec3f &ref2, const Vec3f &ref3) const {
    return ref1 * v[0]*std::sin(v[1])*std::cos(v[2]) + ref2 * v[0]*std::sin(v[1])*std::sin(v[2]) + ref3 * v[0]*std::cos(v[1]);
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
Vec3f Vec3f::pow(double exp) const {
    return Vec3f(std::pow(v[0], exp), std::pow(v[1], exp), std::pow(v[2], exp));
}
Vec3f &Vec3f::pow_inplace(double exp){
    v[0] = std::pow(v[0], exp);
    v[1] = std::pow(v[1], exp);
    v[2] = std::pow(v[2], exp);
    return *this;
}
Vec3f Vec3f::floor() const {
    return Vec3f(std::floor(v[0]), std::floor(v[1]), std::floor(v[2]));
}
Vec3f Vec3f::ceil() const {
    return Vec3f(std::ceil(v[0]), std::ceil(v[1]), std::ceil(v[2]));
}
Vec3f &Vec3f::round_inplace(){
    v[0] = std::round(v[0]);
    v[1] = std::round(v[1]);
    v[2] = std::round(v[2]);
    return *this;
}
Vec3f &Vec3f::clamp(double minimum, double maximum){
    min(maximum);
    max(minimum);
    return *this;
}
std::ostream &operator<<(std::ostream &output, const Vec3f &v) {
    std::cout << '[' << v[0] << ", " << v[1] << ", " << v[2] << ']';
    return output;
} 
Vec3f operator*(const double factor, const Vec3f &v) {
    return Vec3f(v[0] * factor, v[1] * factor, v[2] * factor);
}
double Vec3f::x() const {
    return v[0];
}
double Vec3f::y() const {
    return v[1];
}
double Vec3f::z() const {
    return v[2];
}
double& Vec3f::x() {
    return v[0];
}
double& Vec3f::y() {
    return v[1];
}
double& Vec3f::z() {
    return v[2];
}
double Vec3f::r() const {
    return v[0];
}
double Vec3f::g() const {
    return v[1];
}
double Vec3f::b() const {
    return v[2];
}
double& Vec3f::r() {
    return v[0];
}
double& Vec3f::g() {
    return v[1];
}
double& Vec3f::b() {
    return v[2];
}