#ifndef VEC3F_H
#define VEC3F_H

#include <iostream>

namespace APTracer::Entities {
    class Vec3f {
        public:
            double v[3];
        public:
            Vec3f();
            Vec3f(double x, double y, double z); 
            Vec3f(double x); 
            double &operator[](int index);
            double operator[](int index) const; 
            double &operator[](unsigned int index);
            double operator[](unsigned int index) const; 
            Vec3f operator*(double scale) const;
            Vec3f operator*(const Vec3f &other) const;
            Vec3f operator/(double scale) const;
            Vec3f operator/(const Vec3f &other) const;
            Vec3f operator+(const Vec3f &other) const;
            Vec3f operator+(double factor) const;
            Vec3f operator-(const Vec3f &other) const;
            Vec3f operator-(double factor) const;
            Vec3f operator-() const; 
            bool operator==(const Vec3f &other) const;
            const Vec3f &operator*=(double scale);
            const Vec3f &operator*=(const Vec3f &other);
            const Vec3f &operator/=(double scale);
            const Vec3f &operator/=(const Vec3f &other);
            const Vec3f &operator+=(const Vec3f &other);
            const Vec3f &operator+=(double factor);
            const Vec3f &operator-=(const Vec3f &other); 
            const Vec3f &operator-=(double factor);
            Vec3f &min(const Vec3f &other);
            Vec3f &min(double other);
            Vec3f &max(const Vec3f &other);
            Vec3f &max(double other);
            Vec3f getMin(const Vec3f &other) const;
            Vec3f getMax(const Vec3f &other) const;
            double magnitude() const;
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
}

std::ostream &operator<<(std::ostream &output, const APTracer::Entities::Vec3f &v);

#endif