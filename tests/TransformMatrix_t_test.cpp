#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>

using Catch::Matchers::WithinRel;

TEST_CASE("TransformMatrix_t()", "Proves that the default constructor correctly creates an identity matrix") {
    const APTracer::Entities::TransformMatrix_t transform_matrix;

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

TEST_CASE("unscaled matrix", "Proves that a matrix returns the correct scale") {
    const APTracer::Entities::TransformMatrix_t transform_matrix;

    REQUIRE(transform_matrix.getScale() == 1.0);
}

TEST_CASE("scaled matrix", "Proves that a scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;

    SECTION("Pre-translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));
    }

    SECTION("Pre-rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);
    }

    SECTION("Pre-rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);
    }

    SECTION("Pre-rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);
    }

    SECTION("Pre-rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);
    }

    SECTION("Pre-rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);
    }

    SECTION("Pre-rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);
    }

    SECTION("Pre-rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);
    }

    SECTION("Pre-rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);
    }

    transform_matrix.scale(0.2);

    REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.1));
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.6));
    }

    SECTION("Negative scaling further the matrix changes its scale") {
        transform_matrix.scale(-0.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.02));
    }

    SECTION("Negative axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(-30);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-6.0));
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }
}

TEST_CASE("axis scaled matrix", "Proves that an axis scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;

    SECTION("Pre-translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));
    }

    SECTION("Pre-rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);
    }

    SECTION("Pre-rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);
    }

    SECTION("Pre-rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);
    }

    SECTION("Pre-rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);
    }

    SECTION("Pre-rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);
    }

    SECTION("Pre-rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);
    }

    SECTION("Pre-rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);
    }

    SECTION("Pre-rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);
    }

    transform_matrix.scaleAxis(0.2);

    REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.1));
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.6));
    }

    SECTION("Negative scaling further the matrix changes its scale") {
        transform_matrix.scale(-0.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.02));
    }

    SECTION("Negative axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(-30);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-6.0));
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.2));
    }
}

TEST_CASE("negative scaled matrix", "Proves that a negative scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;

    SECTION("Pre-translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));
    }

    SECTION("Pre-rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);
    }

    SECTION("Pre-rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);
    }

    SECTION("Pre-rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);
    }

    SECTION("Pre-rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);
    }

    SECTION("Pre-rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);
    }

    SECTION("Pre-rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);
    }

    SECTION("Pre-rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);
    }

    SECTION("Pre-rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);
    }

    transform_matrix.scale(-0.2);

    REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.1));
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.6));
    }

    SECTION("Negative scaling further the matrix changes its scale") {
        transform_matrix.scale(-0.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.02));
    }

    SECTION("Negative axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(-30);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(6.0));
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }
}

TEST_CASE("negative axis scaled matrix", "Proves that a negative axis scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;

    SECTION("Pre-translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));
    }

    SECTION("Pre-rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);
    }

    SECTION("Pre-rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);
    }

    SECTION("Pre-rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);
    }

    SECTION("Pre-rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);
    }

    SECTION("Pre-rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);
    }

    SECTION("Pre-rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);
    }

    SECTION("Pre-rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);
    }

    SECTION("Pre-rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);
    }

    transform_matrix.scaleAxis(-0.2);

    REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.1));
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.6));
    }

    SECTION("Negative scaling further the matrix changes its scale") {
        transform_matrix.scale(-0.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(0.02));
    }

    SECTION("Negative axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(-30);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(6.0));
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(APTracer::Entities::Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(APTracer::Entities::Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE_THAT(transform_matrix.getScale(), WithinRel(-0.2));
    }
}
