#include "entities/TransformMatrix_t.hpp"
#include <algorithm>

using APTracer::Entities::TransformMatrix_t;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

TransformMatrix_t::TransformMatrix_t() : matrix_{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} {
    buildInverse();
}

TransformMatrix_t::TransformMatrix_t(
    double i0, double i1, double i2, double i3, double i4, double i5, double i6, double i7, double i8, double i9, double i10, double i11, double i12, double i13, double i14, double i15) :
        matrix_{i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15}, matrix_inverse_{} {
    buildInverse();
}

TransformMatrix_t::TransformMatrix_t(std::array<double, size_> values) : matrix_(values), matrix_inverse_{} {
    buildInverse();
}

auto TransformMatrix_t::rotateXAxis(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{1, 0, 0, 0, 0, cosine, sine, 0, 0, -sine, cosine, 0, 0, 0, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateYAxis(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{cosine, 0, -sine, 0, 0, 1, 0, 0, sine, 0, cosine, 0, 0, 0, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateZAxis(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{cosine, sine, 0, 0, -sine, cosine, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateX(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{1,
                                          0,
                                          0,
                                          0, /* Dunno if those work, pre-multiplied them*/
                                          0,
                                          cosine,
                                          sine,
                                          0,
                                          0,
                                          -sine,
                                          cosine,
                                          0,
                                          0,
                                          matrix_[13] - matrix_[13] * cosine + matrix_[14] * sine,
                                          matrix_[14] - matrix_[14] * cosine - matrix_[13] * sine,
                                          1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateY(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{
        cosine, 0, -sine, 0, 0, 1, 0, 0, sine, 0, cosine, 0, matrix_[12] - matrix_[12] * cosine - matrix_[14] * sine, 0, matrix_[14] - matrix_[14] * cosine + matrix_[12] * sine, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateZ(double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);

    const std::array<double, size_> other{
        cosine, sine, 0, 0, -sine, cosine, 0, 0, 0, 0, 1, 0, matrix_[12] - matrix_[12] * cosine + matrix_[13] * sine, matrix_[13] - matrix_[13] * cosine - matrix_[12] * sine, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotateAxis(const Vec3f& vec, double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);
    const double t      = 1.0 - cosine;

    const Vec3f vec2 = vec.normalize(); // Dunno if needed
    const std::array<double, size_> other{vec2[0] * vec2[0] * t + cosine,
                                          vec2[1] * vec2[0] * t - vec2[2] * sine,
                                          vec2[2] * vec2[0] * t + vec2[1] * sine,
                                          0,
                                          vec2[0] * vec2[1] * t + vec2[2] * sine,
                                          vec2[1] * vec2[1] * t + cosine,
                                          vec2[2] * vec2[1] * t - vec2[0] * sine,
                                          0,
                                          vec2[0] * vec2[2] * t - vec2[1] * sine,
                                          vec2[1] * vec2[2] * t + vec2[0] * sine,
                                          vec2[2] * vec2[2] * t + cosine,
                                          0,
                                          0,
                                          0,
                                          0,
                                          1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::rotate(const Vec3f& vec, double angle) -> TransformMatrix_t& {
    const double cosine = cos(angle);
    const double sine   = sin(angle);
    const double t      = 1.0 - cosine;

    const Vec3f vec2 = vec.normalize(); // Dunno if needed
    const std::array<double, size_> other{
        vec2[0] * vec2[0] * t + cosine,
        vec2[1] * vec2[0] * t - vec2[2] * sine,
        vec2[2] * vec2[0] * t + vec2[1] * sine,
        0,
        vec2[0] * vec2[1] * t + vec2[2] * sine,
        vec2[1] * vec2[1] * t + cosine,
        vec2[2] * vec2[1] * t - vec2[0] * sine,
        0,
        vec2[0] * vec2[2] * t - vec2[1] * sine,
        vec2[1] * vec2[2] * t + vec2[0] * sine,
        vec2[2] * vec2[2] * t + cosine,
        0,
        matrix_[12] - matrix_[12] * (vec2[0] * vec2[0] * t + cosine) - matrix_[13] * (vec2[0] * vec2[1] * t + vec2[2] * sine) - matrix_[14] * (vec2[0] * vec2[2] * t - vec2[1] * sine),
        matrix_[13] - matrix_[12] * (vec2[1] * vec2[0] * t - vec2[2] * sine) - matrix_[13] * (vec2[1] * vec2[1] * t + cosine) - matrix_[14] * (vec2[1] * vec2[2] * t + vec2[0] * sine),
        matrix_[14] - matrix_[12] * (vec2[2] * vec2[0] * t + vec2[1] * sine) - matrix_[13] * (vec2[2] * vec2[1] * t - vec2[0] * sine) - matrix_[14] * (vec2[2] * vec2[2] * t + cosine),
        1};
    // Wow just wow, such a line to write. I assume this is super slow
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::translate(const Vec3f& vec) -> TransformMatrix_t& {
    const std::array<double, size_> other{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, vec[0], vec[1], vec[2], 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) {
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::scaleAxis(const Vec3f& vec) -> TransformMatrix_t& {
    const std::array<double, size_> other{vec[0], 0, 0, 0, 0, vec[1], 0, 0, 0, 0, vec[2], 0, 0, 0, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) { // This can be optimised
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::scaleAxis(double fac) -> TransformMatrix_t& {
    const std::array<double, size_> other{fac, 0, 0, 0, 0, fac, 0, 0, 0, 0, fac, 0, 0, 0, 0, 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) { // This can be optimised
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::scale(const Vec3f& vec) -> TransformMatrix_t& {
    const std::array<double, size_> other{
        vec[0], 0, 0, 0, 0, vec[1], 0, 0, 0, 0, vec[2], 0, matrix_[12] - vec[0] * matrix_[12], matrix_[13] - vec[1] * matrix_[13], matrix_[14] - vec[2] * matrix_[14], 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) { // This can be optimised
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::scale(double fac) -> TransformMatrix_t& {
    const std::array<double, size_> other{fac, 0, 0, 0, 0, fac, 0, 0, 0, 0, fac, 0, matrix_[12] - fac * matrix_[12], matrix_[13] - fac * matrix_[13], matrix_[14] - fac * matrix_[14], 1};
    const std::array<double, size_> matrix(matrix_);

    for (unsigned int j = 0; j < 4; j++) { // This can be optimised
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4 * j + i] = matrix[4 * j] * other[i] + matrix[4 * j + 1] * other[4 + i] + matrix[4 * j + 2] * other[8 + i] + matrix[4 * j + 3] * other[12 + i];
        }
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::reflect(const Vec3f& /*vec*/) -> TransformMatrix_t& { // Needs a 4 component vec, or a distance factor. not used for now
    /*std::array<double, size_> other {1 - 2 * vec[0] * vec[0], -2 * vec[0] * vec[1], -2 * vec[0] * vec[2], -2 * vec[0] * vec[3],
                                    -2 * vec[0] * vec[1], 1 - 2 * vec[1] * vec[1], -2 * vec[1] * vec[2], -2 * vec[1] * vec[3],
                                    -2 * vec[0] * vec[2], -2 * vec[1] * vec[2], 1 - 2 * vec[2] * vec[2], -2 * vec[2] * vec[3],
                                    0, 0, 0, 1};

    for (unsigned int j = 0; j < 4; j++) { // This can be optimised
        for (unsigned int i = 0; i < 4; ++i) {
            matrix_[4*j + i] =    matrix_[4*j] * other[i]
                                + matrix_[4*j + 1] * other[4 + i]
                                + matrix_[4*j + 2] * other[8 + i]
                                + matrix_[4*j + 3] * other[12 + i];
        }
    }*/

    buildInverse();
    return *this;
}

auto TransformMatrix_t::shear(const Vec3f& /*vec*/) -> TransformMatrix_t& { // who the hell uses this
    buildInverse();
    return *this;
}

auto TransformMatrix_t::transpose() -> TransformMatrix_t& {
    matrix_ = {matrix_[0],
               matrix_[4],
               matrix_[8],
               matrix_[12],
               matrix_[1],
               matrix_[5],
               matrix_[9],
               matrix_[13],
               matrix_[2],
               matrix_[6],
               matrix_[10],
               matrix_[14],
               matrix_[3],
               matrix_[7],
               matrix_[11],
               matrix_[15]};

    buildInverse();
    return *this;
}

auto TransformMatrix_t::invert() -> TransformMatrix_t& { // oh boï
    // See https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html

    const std::array<double, size_> transpose{matrix_[0],
                                              matrix_[4],
                                              matrix_[8],
                                              matrix_[12],
                                              matrix_[1],
                                              matrix_[5],
                                              matrix_[9],
                                              matrix_[13],
                                              matrix_[2],
                                              matrix_[6],
                                              matrix_[10],
                                              matrix_[14],
                                              matrix_[3],
                                              matrix_[7],
                                              matrix_[11],
                                              matrix_[15]};

    /* temp array for pairs */
    std::array<double, 12> tmp{transpose[10] * transpose[15],
                               transpose[11] * transpose[14],
                               transpose[9] * transpose[15],
                               transpose[11] * transpose[13],
                               transpose[9] * transpose[14],
                               transpose[10] * transpose[13],
                               transpose[8] * transpose[15],
                               transpose[11] * transpose[12],
                               transpose[8] * transpose[14],
                               transpose[10] * transpose[12],
                               transpose[8] * transpose[13],
                               transpose[9] * transpose[12]};

    // ij order might be messed up for these. If it is, order it: 0 4 8 12 1 5 9 13.
    matrix_[0] = tmp[0] * transpose[5] + tmp[3] * transpose[6] + tmp[4] * transpose[7];
    matrix_[0] -= tmp[1] * transpose[5] + tmp[2] * transpose[6] + tmp[5] * transpose[7];
    matrix_[1] = tmp[1] * transpose[4] + tmp[6] * transpose[6] + tmp[9] * transpose[7];
    matrix_[1] -= tmp[0] * transpose[4] + tmp[7] * transpose[6] + tmp[8] * transpose[7];
    matrix_[2] = tmp[2] * transpose[4] + tmp[7] * transpose[5] + tmp[10] * transpose[7];
    matrix_[2] -= tmp[3] * transpose[4] + tmp[6] * transpose[5] + tmp[11] * transpose[7];
    matrix_[3] = tmp[5] * transpose[4] + tmp[8] * transpose[5] + tmp[11] * transpose[6];
    matrix_[3] -= tmp[4] * transpose[4] + tmp[9] * transpose[5] + tmp[10] * transpose[6];
    matrix_[4] = tmp[1] * transpose[1] + tmp[2] * transpose[2] + tmp[5] * transpose[3];
    matrix_[4] -= tmp[0] * transpose[1] + tmp[3] * transpose[2] + tmp[4] * transpose[3];
    matrix_[5] = tmp[0] * transpose[0] + tmp[7] * transpose[2] + tmp[8] * transpose[3];
    matrix_[5] -= tmp[1] * transpose[0] + tmp[6] * transpose[2] + tmp[9] * transpose[3];
    matrix_[6] = tmp[3] * transpose[0] + tmp[6] * transpose[1] + tmp[11] * transpose[3];
    matrix_[6] -= tmp[2] * transpose[0] + tmp[7] * transpose[1] + tmp[10] * transpose[3];
    matrix_[7] = tmp[4] * transpose[0] + tmp[9] * transpose[1] + tmp[10] * transpose[2];
    matrix_[7] -= tmp[5] * transpose[0] + tmp[8] * transpose[1] + tmp[11] * transpose[2];

    tmp[0] = transpose[2] * transpose[7];
    tmp[1] = transpose[3] * transpose[6];
    tmp[2] = transpose[1] * transpose[7];
    tmp[3] = transpose[3] * transpose[5];
    tmp[4] = transpose[1] * transpose[6];
    tmp[5] = transpose[2] * transpose[5];

    tmp[6]  = transpose[0] * transpose[7];
    tmp[7]  = transpose[3] * transpose[4];
    tmp[8]  = transpose[0] * transpose[6];
    tmp[9]  = transpose[2] * transpose[4];
    tmp[10] = transpose[0] * transpose[5];
    tmp[11] = transpose[1] * transpose[4];

    matrix_[8] = tmp[0] * transpose[13] + tmp[3] * transpose[14] + tmp[4] * transpose[15];
    matrix_[8] -= tmp[1] * transpose[13] + tmp[2] * transpose[14] + tmp[5] * transpose[15];
    matrix_[9] = tmp[1] * transpose[12] + tmp[6] * transpose[14] + tmp[9] * transpose[15];
    matrix_[9] -= tmp[0] * transpose[12] + tmp[7] * transpose[14] + tmp[8] * transpose[15];
    matrix_[10] = tmp[2] * transpose[12] + tmp[7] * transpose[13] + tmp[10] * transpose[15];
    matrix_[10] -= tmp[3] * transpose[12] + tmp[6] * transpose[13] + tmp[11] * transpose[15];
    matrix_[11] = tmp[5] * transpose[12] + tmp[8] * transpose[13] + tmp[11] * transpose[14];
    matrix_[11] -= tmp[4] * transpose[12] + tmp[9] * transpose[13] + tmp[10] * transpose[14];
    matrix_[12] = tmp[2] * transpose[10] + tmp[5] * transpose[11] + tmp[1] * transpose[9];
    matrix_[12] -= tmp[4] * transpose[11] + tmp[0] * transpose[9] + tmp[3] * transpose[10];
    matrix_[13] = tmp[8] * transpose[11] + tmp[0] * transpose[8] + tmp[7] * transpose[10];
    matrix_[13] -= tmp[6] * transpose[10] + tmp[9] * transpose[11] + tmp[1] * transpose[8];
    matrix_[14] = tmp[6] * transpose[9] + tmp[11] * transpose[11] + tmp[3] * transpose[8];
    matrix_[14] -= tmp[10] * transpose[11] + tmp[2] * transpose[8] + tmp[7] * transpose[9];
    matrix_[15] = tmp[10] * transpose[10] + tmp[4] * transpose[8] + tmp[9] * transpose[9];
    matrix_[15] -= tmp[8] * transpose[9] + tmp[11] * transpose[10] + tmp[5] * transpose[8];

    /* determinant */
    const double det = 1.0 / (transpose[0] * matrix_[0] + transpose[1] * matrix_[1] + transpose[2] * matrix_[2] + transpose[3] * matrix_[3]);

    for (auto& element: matrix_) {
        element *= det;
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::neg() -> TransformMatrix_t& {
    for (auto& element: matrix_) {
        element *= -1.0;
    }

    buildInverse();
    return *this;
}

auto TransformMatrix_t::multVec(const Vec3f& vec) const -> Vec3f {
    std::array<double, 4> vec2{};
    for (unsigned int i = 0; i < 4; ++i) {
        vec2[i] = vec[0] * matrix_[i] + vec[1] * matrix_[i + 4] + vec[2] * matrix_[i + 8] + matrix_[i + 12];
    }
    return Vec3f(vec2[0], vec2[1], vec2[2]) / vec2[3];
}

auto TransformMatrix_t::multDir(const Vec3f& vec) const -> Vec3f { // Different than matlab, check.
    // https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html checks for negative afterwards also
    std::array<double, 4> vec2{};
    for (unsigned int i = 0; i < 4; ++i) {
        vec2[i] = vec[0] * matrix_inverse_[i] + vec[1] * matrix_inverse_[i + 4] + vec[2] * matrix_inverse_[i + 8];
    }
    return Vec3f(vec2[0], vec2[1], vec2[2]).normalize_inplace(); /// vec2[3]; // vec2[3] will probably always be 0. website puts at 1 then. if vec2[3] < 0, inverts result.
    // Will maybe have to normalise this. (was not normalised)
}

auto TransformMatrix_t::getScale() const -> double {
    const double norm0 = Vec3f(matrix_[0], matrix_[1], matrix_[2]).magnitude();
    const double norm1 = Vec3f(matrix_[4], matrix_[5], matrix_[6]).magnitude();
    const double norm2 = Vec3f(matrix_[7], matrix_[8], matrix_[9]).magnitude();
    return std::max(std::max(norm0, norm1), norm2);
}

auto TransformMatrix_t::buildInverse() -> void {
    // See https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html

    /* temp array for pairs */
    std::array<double, 12> tmp{matrix_[10] * matrix_[15],
                               matrix_[14] * matrix_[11],
                               matrix_[6] * matrix_[15],
                               matrix_[14] * matrix_[7],
                               matrix_[6] * matrix_[11],
                               matrix_[10] * matrix_[7],
                               matrix_[2] * matrix_[15],
                               matrix_[14] * matrix_[3],
                               matrix_[2] * matrix_[11],
                               matrix_[10] * matrix_[3],
                               matrix_[2] * matrix_[7],
                               matrix_[6] * matrix_[3]};

    // ij order might be messed up for these. If it is, order it: 0 4 8 12 1 5 9 13.
    matrix_inverse_[0] = tmp[0] * matrix_[5] + tmp[3] * matrix_[9] + tmp[4] * matrix_[13];
    matrix_inverse_[0] -= tmp[1] * matrix_[5] + tmp[2] * matrix_[9] + tmp[5] * matrix_[13];
    matrix_inverse_[4] = tmp[1] * matrix_[1] + tmp[6] * matrix_[9] + tmp[9] * matrix_[13];
    matrix_inverse_[4] -= tmp[0] * matrix_[1] + tmp[7] * matrix_[9] + tmp[8] * matrix_[13];
    matrix_inverse_[8] = tmp[2] * matrix_[1] + tmp[7] * matrix_[5] + tmp[10] * matrix_[13];
    matrix_inverse_[8] -= tmp[3] * matrix_[1] + tmp[6] * matrix_[5] + tmp[11] * matrix_[13];
    matrix_inverse_[12] = tmp[5] * matrix_[1] + tmp[8] * matrix_[5] + tmp[11] * matrix_[9];
    matrix_inverse_[12] -= tmp[4] * matrix_[1] + tmp[9] * matrix_[5] + tmp[10] * matrix_[9];
    matrix_inverse_[1] = tmp[1] * matrix_[4] + tmp[2] * matrix_[8] + tmp[5] * matrix_[12];
    matrix_inverse_[1] -= tmp[0] * matrix_[4] + tmp[3] * matrix_[8] + tmp[4] * matrix_[12];
    matrix_inverse_[5] = tmp[0] * matrix_[0] + tmp[7] * matrix_[8] + tmp[8] * matrix_[12];
    matrix_inverse_[5] -= tmp[1] * matrix_[0] + tmp[6] * matrix_[8] + tmp[9] * matrix_[12];
    matrix_inverse_[9] = tmp[3] * matrix_[0] + tmp[6] * matrix_[4] + tmp[11] * matrix_[12];
    matrix_inverse_[9] -= tmp[2] * matrix_[0] + tmp[7] * matrix_[4] + tmp[10] * matrix_[12];
    matrix_inverse_[13] = tmp[4] * matrix_[0] + tmp[9] * matrix_[4] + tmp[10] * matrix_[8];
    matrix_inverse_[13] -= tmp[5] * matrix_[0] + tmp[8] * matrix_[4] + tmp[11] * matrix_[8];

    tmp[0] = matrix_[8] * matrix_[13];
    tmp[1] = matrix_[12] * matrix_[9];
    tmp[2] = matrix_[4] * matrix_[13];
    tmp[3] = matrix_[12] * matrix_[5];
    tmp[4] = matrix_[4] * matrix_[9];
    tmp[5] = matrix_[8] * matrix_[5];

    tmp[6]  = matrix_[0] * matrix_[13];
    tmp[7]  = matrix_[12] * matrix_[1];
    tmp[8]  = matrix_[0] * matrix_[9];
    tmp[9]  = matrix_[8] * matrix_[1];
    tmp[10] = matrix_[0] * matrix_[5];
    tmp[11] = matrix_[4] * matrix_[1];

    matrix_inverse_[2] = tmp[0] * matrix_[7] + tmp[3] * matrix_[11] + tmp[4] * matrix_[15];
    matrix_inverse_[2] -= tmp[1] * matrix_[7] + tmp[2] * matrix_[11] + tmp[5] * matrix_[15];
    matrix_inverse_[6] = tmp[1] * matrix_[3] + tmp[6] * matrix_[11] + tmp[9] * matrix_[15];
    matrix_inverse_[6] -= tmp[0] * matrix_[3] + tmp[7] * matrix_[11] + tmp[8] * matrix_[15];
    matrix_inverse_[10] = tmp[2] * matrix_[3] + tmp[7] * matrix_[7] + tmp[10] * matrix_[15];
    matrix_inverse_[10] -= tmp[3] * matrix_[3] + tmp[6] * matrix_[7] + tmp[11] * matrix_[15];
    matrix_inverse_[14] = tmp[5] * matrix_[3] + tmp[8] * matrix_[7] + tmp[11] * matrix_[11];
    matrix_inverse_[14] -= tmp[4] * matrix_[3] + tmp[9] * matrix_[7] + tmp[10] * matrix_[11];
    matrix_inverse_[3] = tmp[2] * matrix_[10] + tmp[5] * matrix_[14] + tmp[1] * matrix_[6];
    matrix_inverse_[3] -= tmp[4] * matrix_[14] + tmp[0] * matrix_[6] + tmp[3] * matrix_[10];
    matrix_inverse_[7] = tmp[8] * matrix_[14] + tmp[0] * matrix_[2] + tmp[7] * matrix_[10];
    matrix_inverse_[7] -= tmp[6] * matrix_[10] + tmp[9] * matrix_[14] + tmp[1] * matrix_[2];
    matrix_inverse_[11] = tmp[6] * matrix_[6] + tmp[11] * matrix_[14] + tmp[3] * matrix_[2];
    matrix_inverse_[11] -= tmp[10] * matrix_[14] + tmp[2] * matrix_[2] + tmp[7] * matrix_[6];
    matrix_inverse_[15] = tmp[10] * matrix_[10] + tmp[4] * matrix_[2] + tmp[9] * matrix_[6];
    matrix_inverse_[15] -= tmp[8] * matrix_[6] + tmp[11] * matrix_[10] + tmp[5] * matrix_[2];

    /* determinant */
    const double det = 1.0 / (matrix_[0] * matrix_inverse_[0] + matrix_[4] * matrix_inverse_[4] + matrix_[8] * matrix_inverse_[8] + matrix_[12] * matrix_inverse_[12]);

    for (auto& element: matrix_inverse_) {
        element *= det;
    }
}

auto operator<<(std::ostream& output, const TransformMatrix_t& m) -> std::ostream& {
    output << '[' << m.matrix_[0] << ", " << m.matrix_[1] << ", " << m.matrix_[2] << ", " << m.matrix_[3] << "; " << m.matrix_[4] << ", " << m.matrix_[5] << ", " << m.matrix_[6] << ", "
           << m.matrix_[7] << "; " << m.matrix_[8] << ", " << m.matrix_[9] << ", " << m.matrix_[10] << ", " << m.matrix_[11] << "; " << m.matrix_[12] << ", " << m.matrix_[13] << ", " << m.matrix_[14]
           << ", " << m.matrix_[15] << ']';
    return output;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
