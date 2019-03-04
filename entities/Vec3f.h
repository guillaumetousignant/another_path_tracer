#ifndef VEC3F_H
#define VEC3F_H

#include <iostream>
#include <cmath>
using std::ostream;
using std::cout;
using std::cin;
using std::endl; 
class Vec3f {
    public:
        double v[3];
    public:
        Vec3f();
        Vec3f(double x, double y, double z); 
        double &operator[](int index);
        double operator[](int index) const; 
        Vec3f operator*(double scale) const;
        Vec3f operator*(const Vec3f &other) const;
        Vec3f operator/(double scale) const;
        Vec3f operator+(const Vec3f &other) const;
        Vec3f operator+(double factor) const;
        Vec3f operator-(const Vec3f &other) const;
        Vec3f operator-(double factor) const;
        Vec3f operator-() const; 
        bool operator==(const Vec3f &other) const;
        const Vec3f &operator*=(double scale);
        const Vec3f &operator*=(const Vec3f &other);
        const Vec3f &operator/=(double scale);
        const Vec3f &operator+=(const Vec3f &other);
        const Vec3f &operator+=(double factor);
        const Vec3f &operator-=(const Vec3f &other); 
        const Vec3f &operator-=(double factor);
        double magnitude() const;
        double magnitudeSquared() const;
        Vec3f normalize() const;
        double dot(const Vec3f &other) const;
        Vec3f cross(const Vec3f &other) const;
};
/*int main()
{
    Vec3f v1(2,3,-1);
    Vec3f v2(2,5,6);
    cout<<"v1 = "<<v1<<endl;
    cout<<"v2 = "<<v2<<endl;
    cout<<"v1 * 2 = "<<v1*2<<" and v1 / 2 = "<<v1/2<<endl;
    cout<<"v1 + v2 = "<<v1+v2<<endl;
    cout<<"v1 - v2 = "<<v1-v2<<endl;
    cout<<"-v1 = "<<-v1<<endl;
    cout<<"|v1| = "<<v1.magnitude()<<endl;
    cout<<"Normalized v1 = "<<v1.normalize()<<endl;
    cout<<"v1.v2 = "<<v1.dot(v2)<<endl;
    cout<<"v1xv2 = "<<v1.cross(v2)<<endl; 
}*/
#endif