#include "TransformMatrix_t.h"
#include <algorithm>

TransformMatrix_t::TransformMatrix_t() : matrix_{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} {}

TransformMatrix_t::TransformMatrix_t(double i0, double i1, double i2, double i3, double i4, double i5, double i6, double i7, double i8, double i9, double i10, double i11, double i12, double i13, double i14, double i15) 
    : matrix_{i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15} {}

TransformMatrix_t::TransformMatrix_t(const TransformMatrix_t &other)
    : matrix_{other.matrix_[0], other.matrix_[1], other.matrix_[2], other.matrix_[3],
              other.matrix_[4], other.matrix_[5], other.matrix_[6], other.matrix_[7],
              other.matrix_[8], other.matrix_[9], other.matrix_[10], other.matrix_[11],
              other.matrix_[12], other.matrix_[13], other.matrix_[14], other.matrix_[15]} {}

TransformMatrix_t::~TransformMatrix_t(){}

TransformMatrix_t&  TransformMatrix_t::rotateXAxis(double angle){
    const double other[16] = {1, 0, 0, 0,
                        0, cos(angle), sin(angle), 0,
                        0, -sin(angle), cos(angle), 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateYAxis(double angle){
    const double other[16] = {cos(angle), 0, -sin(angle), 0,
                        0, 1, 0, 0,
                        sin(angle), 0, cos(angle), 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateZAxis(double angle){
    const double other[16] = {cos(angle), sin(angle), 0, 0,
                        -sin(angle), cos(angle), 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateX(double angle){
    const double other[16] = {1, 0, 0, 0, /* Dunno if those work, pre-multiplied them*/
                    0, cos(angle), sin(angle), 0,
                    0, -sin(angle), cos(angle), 0,
                    0, matrix_[13] - matrix_[13]*cos(angle) + matrix_[14]*sin(angle), matrix_[14] - matrix_[14]*cos(angle) - matrix_[13]*sin(angle), 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateY(double angle){
    const double other[16] = {cos(angle), 0, -sin(angle), 0,
                        0, 1, 0, 0,
                        sin(angle), 0, cos(angle), 0,
                        matrix_[12] - matrix_[12]*cos(angle) - matrix_[14]*sin(angle), 0, matrix_[14] - matrix_[14]*cos(angle) + matrix_[12]*sin(angle), 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateZ(double angle){
    const double other[16] = {cos(angle), sin(angle), 0, 0,
                        -sin(angle), cos(angle), 0, 0,
                        0, 0, 1, 0,
                        matrix_[12] - matrix_[12]*cos(angle) + matrix_[13]*sin(angle), matrix_[13] - matrix_[13]*cos(angle) - matrix_[12]*sin(angle), 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotateAxis(const Vec3f &vec, double angle){
    const Vec3f vec2 = vec.normalize(); // Dunno if needed
    const double other[16] = {vec2[0] * vec2[0] * (1 - cos(angle)) + cos(angle), vec2[1] * vec2[0] * (1 - cos(angle)) - vec2[2] * sin(angle), vec2[2] * vec2[0] * (1 - cos(angle)) + vec2[1] * sin(angle), 0,
                        vec2[0] * vec2[1] * (1 - cos(angle)) + vec2[2] * sin(angle), vec2[1] * vec2[1] * (1 - cos(angle)) + cos(angle), vec2[2] * vec2[1] * (1 - cos(angle)) - vec2[0] * sin(angle), 0,
                        vec2[0] * vec2[2] * (1 - cos(angle)) - vec2[0] * sin(angle), vec2[0] * vec2[1] * (1 - cos(angle)) + vec2[0] * sin(angle), vec2[2] * vec2[2] * (1 - cos(angle)) + cos(angle), 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::rotate(const Vec3f &vec, double angle){
    const Vec3f vec2 = vec.normalize(); // Dunno if needed
    const double other[16] = {vec2[0] * vec2[0] * (1 - cos(angle)) + cos(angle), vec2[1] * vec2[0] * (1 - cos(angle)) - vec2[2] * sin(angle), vec2[2] * vec2[0] * (1 - cos(angle)) + vec2[1] * sin(angle), 0,
                        vec2[0] * vec2[1] * (1 - cos(angle)) + vec2[2] * sin(angle), vec2[1] * vec2[1] * (1 - cos(angle)) + cos(angle), vec2[2] * vec2[1] * (1 - cos(angle)) - vec2[0] * sin(angle), 0,
                        vec2[0] * vec2[2] * (1 - cos(angle)) - vec2[0] * sin(angle), vec2[0] * vec2[1] * (1 - cos(angle)) + vec2[0] * sin(angle), vec2[2] * vec2[2] * (1 - cos(angle)) + cos(angle), 0,
                        matrix_[12] + matrix_[14]*(vec2[0]*sin(angle) + vec2[0]*vec2[2]*(cos(angle) - 1)) - matrix_[13]*(vec2[2]*sin(angle) - vec2[0]*vec2[1]*(cos(angle) - 1)) - matrix_[12]*((1 - cos(angle))*vec2[0]*vec2[0] + cos(angle)), matrix_[13] + matrix_[12]*(vec2[2]*sin(angle) + vec2[0]*vec2[1]*(cos(angle) - 1)) - matrix_[14]*(vec2[0]*sin(angle) - vec2[0]*vec2[1]*(cos(angle) - 1)) - matrix_[13]*((1 - cos(angle))*vec2[1]*vec2[1] + cos(angle)), matrix_[14] - matrix_[12]*(vec2[1]*sin(angle) - vec2[0]*vec2[2]*(cos(angle) - 1)) + matrix_[13]*(vec2[0]*sin(angle) + vec2[1]*vec2[2]*(cos(angle) - 1)) - matrix_[14]*((1 - cos(angle))*vec2[2]*vec2[2] + cos(angle)), 1};
                        // Wow just wow, such a line to write. I assume this is super slow
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::translate(const Vec3f &vec){
    const double other[16] = {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        vec[0], vec[1], vec[2], 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::scaleAxis(const Vec3f &vec){
    const double other[16] = {vec[0], 0, 0, 0,
                        0, vec[1], 0, 0,
                        0, 0, vec[2], 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){ // This can be optimised
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::scaleAxis(double fac){
    const double other[16] = {fac, 0, 0, 0,
                        0, fac, 0, 0,
                        0, 0, fac, 0,
                        0, 0, 0, 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){ // This can be optimised
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::scale(const Vec3f &vec){
    const double other[16] = {vec[0], 0, 0, 0,
                        0, vec[1], 0, 0,
                        0, 0, vec[2], 0,
                        matrix_[12] - vec[0]*matrix_[12], matrix_[13] - vec[1]*matrix_[13], matrix_[14] - vec[2]*matrix_[14], 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){ // This can be optimised
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::scale(double fac){
    const double other[16] = {fac, 0, 0, 0,
                        0, fac, 0, 0,
                        0, 0, fac, 0,
                        matrix_[12] - fac*matrix_[12], matrix_[13] - fac*matrix_[13], matrix_[14] - fac*matrix_[14], 1};
    double matrix[16];
    std::copy(std::begin(matrix_), std::end(matrix_), std::begin(matrix));

    for (unsigned int j = 0; j < 4; j++){ // This can be optimised
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix[4*j] * other[i]
                                + matrix[4*j + 1] * other[4 + i]
                                + matrix[4*j + 2] * other[8 + i]
                                + matrix[4*j + 3] * other[12 + i];
        }
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::reflect(const Vec3f &vec){ // Needs a 4 component vec, or a distance factor. not used for now
    /*double other[16] = {1 - 2 * vec[0] * vec[0], -2 * vec[0] * vec[1], -2 * vec[0] * vec[2], -2 * vec[0] * vec[3],
                        -2 * vec[0] * vec[1], 1 - 2 * vec[1] * vec[1], -2 * vec[1] * vec[2], -2 * vec[1] * vec[3],
                        -2 * vec[0] * vec[2], -2 * vec[1] * vec[2], 1 - 2 * vec[2] * vec[2], -2 * vec[2] * vec[3],
                        0, 0, 0, 1};

    for (unsigned int j = 0; j < 4; j++){ // This can be optimised
        for (unsigned int i = 0; i < 4; i++){
            matrix_[4*j + i] =    matrix_[4*j] * other[i]
                                + matrix_[4*j + 1] * other[4 + i]
                                + matrix_[4*j + 2] * other[8 + i]
                                + matrix_[4*j + 3] * other[12 + i];
        }
    }*/
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::shear(const Vec3f &vec){ // who the hell uses this
    return *this;
}

TransformMatrix_t& TransformMatrix_t::transpose(){
    const double other[16] = {matrix_[0], matrix_[4], matrix_[8], matrix_[12],
                        matrix_[1], matrix_[5], matrix_[9], matrix_[13],
                        matrix_[2], matrix_[6], matrix_[10], matrix_[14],
                        matrix_[3], matrix_[7], matrix_[11], matrix_[15]};
    
    for (unsigned int i = 0; i < 16; i++){ // This can be optimised
        matrix_[i] = other[i];
    }
    return *this;
}

TransformMatrix_t&  TransformMatrix_t::invert(){ // oh boï
    // See https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html

    double tmp[12]; /* temp array for pairs */
    double det; /* determinant */
    const double transpose[16] = {matrix_[0], matrix_[4], matrix_[8], matrix_[12],
                            matrix_[1], matrix_[5], matrix_[9], matrix_[13],
                            matrix_[2], matrix_[6], matrix_[10], matrix_[14],
                            matrix_[3], matrix_[7], matrix_[11], matrix_[15]};

    tmp[0] = transpose[10] * transpose[15];
    tmp[1] = transpose[11] * transpose[14];
    tmp[2] = transpose[9] * transpose[15];
    tmp[3] = transpose[11] * transpose[13];
    tmp[4] = transpose[9] * transpose[14];
    tmp[5] = transpose[10] * transpose[13];
    tmp[6] = transpose[8] * transpose[15];
    tmp[7] = transpose[11] * transpose[12];
    tmp[8] = transpose[8] * transpose[14];
    tmp[9] = transpose[10] * transpose[12];
    tmp[10] = transpose[8] * transpose[13];
    tmp[11] = transpose[9] * transpose[12];

    // ij order might be messed up for these. If it is, order it: 0 4 8 12 1 5 9 13.
    matrix_[0] = tmp[0]*transpose[5] + tmp[3]*transpose[6] + tmp[4]*transpose[7];
    matrix_[0] -= tmp[1]*transpose[5] + tmp[2]*transpose[6] + tmp[5]*transpose[7];
    matrix_[1] = tmp[1]*transpose[4] + tmp[6]*transpose[6] + tmp[9]*transpose[7];
    matrix_[1] -= tmp[0]*transpose[4] + tmp[7]*transpose[6] + tmp[8]*transpose[7];
    matrix_[2] = tmp[2]*transpose[4] + tmp[7]*transpose[5] + tmp[10]*transpose[7];
    matrix_[2] -= tmp[3]*transpose[4] + tmp[6]*transpose[5] + tmp[11]*transpose[7];
    matrix_[3] = tmp[5]*transpose[4] + tmp[8]*transpose[5] + tmp[11]*transpose[6];
    matrix_[3] -= tmp[4]*transpose[4] + tmp[9]*transpose[5] + tmp[10]*transpose[6];
    matrix_[4] = tmp[1]*transpose[1] + tmp[2]*transpose[2] + tmp[5]*transpose[3];
    matrix_[4] -= tmp[0]*transpose[1] + tmp[3]*transpose[2] + tmp[4]*transpose[3];
    matrix_[5] = tmp[0]*transpose[0] + tmp[7]*transpose[2] + tmp[8]*transpose[3];
    matrix_[5] -= tmp[1]*transpose[0] + tmp[6]*transpose[2] + tmp[9]*transpose[3];
    matrix_[6] = tmp[3]*transpose[0] + tmp[6]*transpose[1] + tmp[11]*transpose[3];
    matrix_[6] -= tmp[2]*transpose[0] + tmp[7]*transpose[1] + tmp[10]*transpose[3];
    matrix_[7] = tmp[4]*transpose[0] + tmp[9]*transpose[1] + tmp[10]*transpose[2];
    matrix_[7] -= tmp[5]*transpose[0] + tmp[8]*transpose[1] + tmp[11]*transpose[2];

    tmp[0] = transpose[2]*transpose[7];
    tmp[1] = transpose[3]*transpose[6];
    tmp[2] = transpose[1]*transpose[7];
    tmp[3] = transpose[3]*transpose[5];
    tmp[4] = transpose[1]*transpose[6];
    tmp[5] = transpose[2]*transpose[5];

    tmp[6] = transpose[0]*transpose[7];
    tmp[7] = transpose[3]*transpose[4];
    tmp[8] = transpose[0]*transpose[6];
    tmp[9] = transpose[2]*transpose[4];
    tmp[10] = transpose[0]*transpose[5];
    tmp[11] = transpose[1]*transpose[4];

    matrix_[8] = tmp[0]*transpose[13] + tmp[3]*transpose[14] + tmp[4]*transpose[15];
    matrix_[8] -= tmp[1]*transpose[13] + tmp[2]*transpose[14] + tmp[5]*transpose[15];
    matrix_[9] = tmp[1]*transpose[12] + tmp[6]*transpose[14] + tmp[9]*transpose[15];
    matrix_[9] -= tmp[0]*transpose[12] + tmp[7]*transpose[14] + tmp[8]*transpose[15];
    matrix_[10] = tmp[2]*transpose[12] + tmp[7]*transpose[13] + tmp[10]*transpose[15];
    matrix_[10] -= tmp[3]*transpose[12] + tmp[6]*transpose[13] + tmp[11]*transpose[15];
    matrix_[11] = tmp[5]*transpose[12] + tmp[8]*transpose[13] + tmp[11]*transpose[14];
    matrix_[11] -= tmp[4]*transpose[12] + tmp[9]*transpose[13] + tmp[10]*transpose[14];
    matrix_[12] = tmp[2]*transpose[10] + tmp[5]*transpose[11] + tmp[1]*transpose[9];
    matrix_[12] -= tmp[4]*transpose[11] + tmp[0]*transpose[9] + tmp[3]*transpose[10];
    matrix_[13] = tmp[8]*transpose[11] + tmp[0]*transpose[8] + tmp[7]*transpose[10];
    matrix_[13] -= tmp[6]*transpose[10] + tmp[9]*transpose[11] + tmp[1]*transpose[8];
    matrix_[14] = tmp[6]*transpose[9] + tmp[11]*transpose[11] + tmp[3]*transpose[8];
    matrix_[14] -= tmp[10]*transpose[11] + tmp[2]*transpose[8] + tmp[7]*transpose[9];
    matrix_[15] = tmp[10]*transpose[10] + tmp[4]*transpose[8] + tmp[9]*transpose[9];
    matrix_[15] -= tmp[8]*transpose[9] + tmp[11]*transpose[10] + tmp[5]*transpose[8];

    det = transpose[0]*matrix_[0]+transpose[1]*matrix_[1]+transpose[2]*matrix_[2]+transpose[3]*matrix_[3];
    det = 1.0/det;

    for (unsigned int i = 0; i < 16; i++){
        matrix_[i] *= det;
    }

    return *this;
}

TransformMatrix_t&  TransformMatrix_t::neg(){
    for (unsigned int i = 0; i < 16; i++){
        matrix_[i] *= -1;
    }
    return *this;
}

Vec3f TransformMatrix_t::multVec(const Vec3f &vec) const{
    double vec2[4];
    for (unsigned int i = 0; i < 4; i++){
        vec2[i] = vec[0] * matrix_[i] + vec[1] * matrix_[i + 4] + vec[2] * matrix_[i + 8] + matrix_[i + 12];
    }
    return Vec3f(vec2[0], vec2[1], vec2[2])/vec2[3];
}

Vec3f TransformMatrix_t::multDir(const Vec3f &vec) const{ // Different than matlab, check. 
    // https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html checks for negative afterwards also
    double vec2[4];
    for (unsigned int i = 0; i < 4; i++){
        vec2[i] = vec[0] * matrix_[i] + vec[1] * matrix_[i + 4] + vec[2] * matrix_[i + 8];
    }
    return Vec3f(vec2[0], vec2[1], vec2[2]).normalize(); ///vec2[3]; // vec2[3] will probably always be 0. website puts at 1 then. if vec2[3] < 0, inverts result.
    // Will maybe have to normalise this. (was not normalised)
}

TransformMatrix_t TransformMatrix_t::transformDir() const{
    return TransformMatrix_t(*this).invert().transpose();
}

double TransformMatrix_t::getScale() const{
    const double norm0 = Vec3f(matrix_[0], matrix_[1], matrix_[2]).magnitude();
    const double norm1 = Vec3f(matrix_[4], matrix_[5], matrix_[6]).magnitude();
    const double norm2 = Vec3f(matrix_[7], matrix_[8], matrix_[9]).magnitude();
    return std::max(std::max(norm0, norm1), norm2);
}