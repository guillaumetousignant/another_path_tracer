#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("TransformMatrix_t()", "Proves that the default constructor correctly creates an identity matrix") {
    const APTracer::Entities::TransformMatrix_t transform_matrix{};

    REQUIRE(transform_matrix.matrix_[0] == 1.0);
    REQUIRE(transform_matrix.matrix_[1] == 0.0);
    REQUIRE(transform_matrix.matrix_[2] == 0.0);
    REQUIRE(transform_matrix.matrix_[3] == 0.0);
    REQUIRE(transform_matrix.matrix_[4] == 0.0);
    REQUIRE(transform_matrix.matrix_[5] == 1.0);
    REQUIRE(transform_matrix.matrix_[6] == 0.0);
    REQUIRE(transform_matrix.matrix_[7] == 0.0);
    REQUIRE(transform_matrix.matrix_[8] == 0.0);
    REQUIRE(transform_matrix.matrix_[9] == 0.0);
    REQUIRE(transform_matrix.matrix_[10] == 1.0);
    REQUIRE(transform_matrix.matrix_[11] == 0.0);
    REQUIRE(transform_matrix.matrix_[12] == 0.0);
    REQUIRE(transform_matrix.matrix_[13] == 0.0);
    REQUIRE(transform_matrix.matrix_[14] == 0.0);
    REQUIRE(transform_matrix.matrix_[15] == 1.0);
}

TEST_CASE("scale(0.2) getScale()", "Proves that a scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(0.2);

    REQUIRE(transform_matrix.getScale() == 0.2);
}

TEST_CASE("scale(0.2, 0.1, -0.15) getScale()", "Proves that a vector scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(APTracer::Entities::Vec3f(0.2, 0.1, -0.15));

    REQUIRE(transform_matrix.getScale() == 0.2);
}

TEST_CASE("scale(-0.2) getScale()", "Proves that a negative scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(-0.2);

    REQUIRE(transform_matrix.getScale() == -0.2);
}

TEST_CASE("scale(0.2, 0.1, -0.35) getScale()", "Proves that a negative vector scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(APTracer::Entities::Vec3f(0.2, 0.1, -0.35));

    REQUIRE(transform_matrix.getScale() == -0.35);
}

TEST_CASE("scaleAxis(0.2) getScale()", "Proves that an axis scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(0.2);

    REQUIRE(transform_matrix.getScale() == 0.2);
}

TEST_CASE("scaleAxis(0.2, 0.1, -0.15) getScale()", "Proves that axis vector scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(APTracer::Entities::Vec3f(0.2, 0.1, -0.15));

    REQUIRE(transform_matrix.getScale() == 0.2);
}

TEST_CASE("scaleAxis(-0.2) getScale()", "Proves that an axis negative scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(-0.2);

    REQUIRE(transform_matrix.getScale() == -0.2);
}

TEST_CASE("scaleAxis(0.2, 0.1, -0.35) getScale()", "Proves that axis vector negative scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(APTracer::Entities::Vec3f(0.2, 0.1, -0.35));

    REQUIRE(transform_matrix.getScale() == -0.35);
}
