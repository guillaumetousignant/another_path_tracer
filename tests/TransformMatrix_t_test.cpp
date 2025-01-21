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

TEST_CASE("scaled matrix", "Proves that a scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(0.2);

    REQUIRE(transform_matrix.getScale() == 0.2);

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE(transform_matrix.getScale() == 0.1);
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE(transform_matrix.getScale() == 0.6);
    }

    SECTION("Vector scaling further the matrix changes its scale") {
        transform_matrix.scale(Vec3f(0.1, 0.25, -0.15));

        REQUIRE(transform_matrix.getScale() == 0.05);
    }

    SECTION("Vector axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(Vec3f(-3, 0.25, 5));

        REQUIRE(transform_matrix.getScale() == 1);
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }
}

TEST_CASE("vector scaled matrix", "Proves that a vector scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scale(APTracer::Entities::Vec3f(0.2, 0.1, -0.15));

    REQUIRE(transform_matrix.getScale() == 0.2);

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE(transform_matrix.getScale() == 0.1);
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE(transform_matrix.getScale() == 0.6);
    }

    SECTION("Vector scaling further the matrix changes its scale") {
        transform_matrix.scale(Vec3f(0.1, 0.25, -0.15));

        REQUIRE(transform_matrix.getScale() == 0.05);
    }

    SECTION("Vector axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(Vec3f(-3, 0.25, 5));

        REQUIRE(transform_matrix.getScale() == 1);
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }
}

TEST_CASE("axis scaled matrix", "Proves that an axis scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(0.2);

    REQUIRE(transform_matrix.getScale() == 0.2);

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE(transform_matrix.getScale() == 0.1);
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE(transform_matrix.getScale() == 0.6);
    }

    SECTION("Vector scaling further the matrix changes its scale") {
        transform_matrix.scale(Vec3f(0.1, 0.25, -0.15));

        REQUIRE(transform_matrix.getScale() == 0.05);
    }

    SECTION("Vector axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(Vec3f(-3, 0.25, 5));

        REQUIRE(transform_matrix.getScale() == 1);
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }
}

TEST_CASE("vector axis scaled matrix", "Proves that a vector axis scaled matrix returns the correct scale") {
    APTracer::Entities::TransformMatrix_t transform_matrix;
    transform_matrix.scaleAxis(APTracer::Entities::Vec3f(0.2, 0.1, -0.15));

    REQUIRE(transform_matrix.getScale() == 0.2);

    SECTION("Scaling further the matrix changes its scale") {
        transform_matrix.scale(0.5);

        REQUIRE(transform_matrix.getScale() == 0.1);
    }

    SECTION("Axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(3);

        REQUIRE(transform_matrix.getScale() == 0.6);
    }

    SECTION("Vector scaling further the matrix changes its scale") {
        transform_matrix.scale(Vec3f(0.1, 0.25, -0.15));

        REQUIRE(transform_matrix.getScale() == 0.05);
    }

    SECTION("Vector axis scaling further the matrix changes its scale") {
        transform_matrix.scaleAxis(Vec3f(-3, 0.25, 5));

        REQUIRE(transform_matrix.getScale() == 1);
    }

    SECTION("Translating the matrix doesn't change its scale") {
        transform_matrix.translate(APTracer::Entities::Vec3f(-5, 0.1, -0.3));

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the x axis doesn't change its scale") {
        transform_matrix.rotateXAxis(1.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the y axis doesn't change its scale") {
        transform_matrix.rotateYAxis(99);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around the z axis doesn't change its scale") {
        transform_matrix.rotateZAxis(-0.002);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around x doesn't change its scale") {
        transform_matrix.rotateX(-2.33);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around y doesn't change its scale") {
        transform_matrix.rotateY(3.1);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around z doesn't change its scale") {
        transform_matrix.rotateZ(-76);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix around an axis doesn't change its scale") {
        transform_matrix.rotateAxis(Vec3f(1.5, -0.2, 7), 0.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }

    SECTION("Rotating the matrix doesn't change its scale") {
        transform_matrix.rotate(Vec3f(0.7, -5.2, 7), -7.3);

        REQUIRE(transform_matrix.getScale() == 0.2);
    }
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
