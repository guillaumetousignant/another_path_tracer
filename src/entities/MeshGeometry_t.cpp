#include "entities/MeshGeometry_t.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using APTracer::Entities::MeshGeometry_t;
using APTracer::Entities::Vec3f;

MeshGeometry_t::MeshGeometry_t(const std::string& filename) {
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (ext == "obj") {
        const std::vector<std::array<bool, 3>> missing_normals = readObj(filename);
        build_missing_normals(missing_normals);
    }
    else if (ext == "su2") {
        const std::vector<std::array<bool, 3>> missing_normals = readSU2(filename);
        build_missing_normals(missing_normals);
    }
}

auto MeshGeometry_t::readObj(const std::string& filename) -> std::vector<std::array<bool, 3>> {
    size_t n_vertices                   = 0;
    size_t n_vertex_texture_coordinates = 0;
    size_t n_vertex_normals             = 0;
    size_t n_faces                      = 0;
    unsigned int nsides                 = 0;
    std::string line;
    std::string token;
    std::string dummy;

    std::ifstream meshfile(filename);
    if (!meshfile.is_open()) {
        std::cerr << "Error: file '" << filename << "' could not be opened. Exiting." << std::endl;
        return {};
    }

    // Getting number of elements
    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        liness >> token;

        if (token == "v") {
            ++n_vertices;
        }
        else if (token == "vt") {
            ++n_vertex_texture_coordinates;
        }
        else if (token == "vn") {
            ++n_vertex_normals;
        }
        else if (token == "f") {
            nsides = 0;
            while (liness >> dummy) {
                ++nsides;
            }
            n_faces += nsides - 2;
        }
    }

    // Getting normals, vertex coordinates and texture coordinates
    size_t vertex_counter                    = 0;
    size_t vertex_texture_coordinate_counter = 0;
    size_t vertex_normal_counter             = 0;
    double val0                              = 0.0;
    double val1                              = 0.0;
    double val2                              = 0.0;

    std::vector<Vec3f> vertices(n_vertices);
    std::vector<std::array<double, 2>> vertex_texture_coordinates(n_vertex_texture_coordinates);
    std::vector<Vec3f> vertex_normals(n_vertex_normals);

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        liness >> token;

        if (token == "v") {
            liness >> val0 >> val1 >> val2;
            vertices[vertex_counter] = Vec3f(val0, val1, val2);
            ++vertex_counter;
        }
        else if (token == "vt") {
            liness >> val0 >> val1;
            vertex_texture_coordinates[vertex_texture_coordinate_counter] = {val0, val1};
            ++vertex_texture_coordinate_counter;
        }
        else if (token == "vn") {
            liness >> val0 >> val1 >> val2;
            vertex_normals[vertex_normal_counter] = Vec3f(val0, val1, val2);
            ++vertex_normal_counter;
        }
    }

    // Filling faces
    size_t face_counter = 0;
    std::string material;
    std::array<std::string, 3> tokens;
    std::string value;
    size_t pos = 0;

    mat_ = std::vector<std::string>(n_faces);
    v_   = std::vector<std::array<Vec3f, 3>>(mat_.size());
    vt_  = std::vector<std::array<std::array<double, 2>, 3>>(mat_.size());
    vn_  = std::vector<std::array<Vec3f, 3>>(mat_.size());
    std::vector<std::array<bool, 3>> missing_normals(mat_.size());

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        liness >> token;
        if (token == "f") {
            liness >> tokens[0] >> tokens[1];
            while (liness >> tokens[2]) {
                for (unsigned int i = 0; i < 3; ++i) {
                    value              = tokens[i];
                    mat_[face_counter] = material;
                    pos                = value.find('/');
                    if (pos == std::string::npos) {
                        v_[face_counter][i]              = vertices[std::stoi(value, nullptr) - 1];
                        vt_[face_counter][i]             = {0.0, 0.0};
                        vn_[face_counter][i]             = Vec3f(0.0);
                        missing_normals[face_counter][i] = true;
                    }
                    else {
                        v_[face_counter][i] = vertices[std::stoi(value.substr(0, pos), nullptr) - 1];
                        value.erase(0, pos + 1);

                        pos = value.find('/');
                        if (pos == std::string::npos) {
                            vt_[face_counter][i]             = vertex_texture_coordinates[std::stoi(value, nullptr) - 1];
                            vn_[face_counter][i]             = Vec3f(0.0);
                            missing_normals[face_counter][i] = true;
                        }
                        else {
                            if (pos == 0) {
                                vt_[face_counter][i] = {0.0, 0.0};
                            }
                            else {
                                vt_[face_counter][i] = vertex_texture_coordinates[std::stoi(value.substr(0, pos), nullptr) - 1];
                            }
                            value.erase(0, pos + 1);
                            vn_[face_counter][i]             = vertex_normals[std::stoi(value, nullptr) - 1];
                            missing_normals[face_counter][i] = false;
                        }
                    }
                }
                ++face_counter;
                tokens[1] = tokens[2];
            }
        }
        else if (token == "usemtl") { // check if there is :
            liness >> tokens[0];
            pos = tokens[0].find(':');
            if (pos != std::string::npos) {
                tokens[0].erase(0, pos + 1);
            }
            material = tokens[0];
        }
    }

    meshfile.close();
    return missing_normals;
}

auto MeshGeometry_t::readSU2(const std::string& filename) -> std::vector<std::array<bool, 3>> {
    size_t n_vertices = 0;
    size_t n_faces    = 0;
    std::string line;
    std::string token;
    unsigned int value = 0;
    bool wall_started  = false;

    std::ifstream meshfile(filename);
    if (!meshfile.is_open()) {
        std::cerr << "Error: file '" << filename << "' could not be opened. Exiting." << std::endl;
        return {};
    }

    // Getting number of elements
    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        liness >> token;

        if (token == "NPOIN=") {
            liness >> value;
            wall_started = false;
            n_vertices += value;
        }
        else if (token == "MARKER_TAG=") {
            liness >> token;
            if (token == "WALL") {
                wall_started = true;
                std::getline(meshfile, line);
            }
            else {
                wall_started = false;
            }
        }
        else if (token.empty()) {
            wall_started = false;
        }
        else if (token == "5") {
            if (wall_started) {
                ++n_faces;
            }
        }
        else if (token == "9") {
            if (wall_started) {
                n_faces += 2;
            }
        }
    }

    // Getting normals, vertex coordinates and texture coordinates
    size_t vertex_counter = 0;
    double val0           = 0.0;
    double val1           = 0.0;
    double val2           = 0.0;
    bool points_started   = false;

    std::vector<Vec3f> vertices(n_vertices);

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        if ((line == "\r") || (line.empty())) {
            token = "";
        }
        else {
            liness >> token;
        }

        if (token == "NELEM=" || token == "NMARK=" || token == "MARKER_TAG=") {
            points_started = false;
        }
        else if (token == "NPOIN=") {
            points_started = true;
            std::getline(meshfile, line);
        }

        if (points_started && (!token.empty())) {
            std::istringstream liness2(line);
            liness2 >> val0 >> val1 >> val2;
            vertices[vertex_counter] = Vec3f(val0, val1, val2);
            ++vertex_counter;
        }
    }

    // Filling faces
    size_t face_counter = 0;
    std::array<size_t, 3> tokens{};
    wall_started = false;
    std::string marker_token;

    mat_ = std::vector<std::string>(n_faces);
    v_   = std::vector<std::array<Vec3f, 3>>(mat_.size());
    vt_  = std::vector<std::array<std::array<double, 2>, 3>>(mat_.size());
    vn_  = std::vector<std::array<Vec3f, 3>>(mat_.size());
    std::vector<std::array<bool, 3>> missing_normals(mat_.size(), {true, true, true});

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)) {
        std::istringstream liness(line);
        if ((line == "\r") || (line.empty())) {
            token = "";
        }
        else {
            liness >> token;
        }

        if (token == "MARKER_TAG=") {
            liness >> marker_token;
            if (marker_token != "WALL") {
                wall_started = false;
            }
        }
        else if (token.empty() || token == "NMARK=" || token == "NELEM=" || token == "MARKER_ELEMS=") {
            wall_started = false;
        }

        if (wall_started) {
            if (token == "5") {
                for (unsigned int i = 0; i < 3; ++i) {
                    liness >> tokens[i];
                    v_[face_counter][i]  = vertices[tokens[i]];
                    mat_[face_counter]   = "";
                    vt_[face_counter][i] = {0.0, 0.0};
                    vn_[face_counter][i] = Vec3f(0.0);
                }
                ++face_counter;
            }
            else if (token == "9") {
                for (unsigned int i = 0; i < 3; ++i) {
                    liness >> tokens[i];
                    v_[face_counter][i]  = vertices[tokens[i]];
                    mat_[face_counter]   = "";
                    vt_[face_counter][i] = {0.0, 0.0};
                    vn_[face_counter][i] = Vec3f(0.0);
                }
                ++face_counter;
                tokens[1] = tokens[2];
                liness >> tokens[2];
                for (unsigned int i = 0; i < 3; ++i) {
                    v_[face_counter][i]  = vertices[tokens[i]];
                    mat_[face_counter]   = "";
                    vt_[face_counter][i] = {0.0, 0.0};
                    vn_[face_counter][i] = Vec3f(0.0);
                }
                ++face_counter;
            }
        }

        if (token == "MARKER_TAG=") {
            if (marker_token == "WALL") {
                wall_started = true;
                std::getline(meshfile, line);
            }
        }
    }

    meshfile.close();
    return missing_normals;
}

auto MeshGeometry_t::build_missing_normals(const std::vector<std::array<bool, 3>>& normals_to_build) -> void {
    for (size_t i = 0; i < v_.size(); ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            if (normals_to_build[i][j]) {
                vn_[i][j] = (v_[i][1] - v_[i][0]).cross(v_[i][2] - v_[i][0]).normalize_inplace();
            }
        }
    }
}
