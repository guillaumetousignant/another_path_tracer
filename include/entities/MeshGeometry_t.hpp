#ifndef APTRACER_ENTITIES_MESHGEOMETRY_T_HPP
#define APTRACER_ENTITIES_MESHGEOMETRY_T_HPP

#include "entities/Vec3f.hpp"
#include <array>
#include <string>
#include <vector>

namespace APTracer { namespace Entities {

    /**
     * @brief The mesh geometry class represents a geometry made up of points and triangular faces.
     *
     * Mesh geometries represent a single geometry without any transformation. Multiple meshes can point to the same mesh geometry
     * while using different transformations, enabling instantiating and saving ressources. This class is constructed from geometry
     * input files. Currently, .obj and .su2 files are supported.
     */
    class MeshGeometry_t {
        public:
            /**
             * @brief Construct a new MeshGeometry_t object from a geometry input file.
             *
             * The input file can have non-triangular faces, as those are triangularised. For .su2 files, the faces in the 'WALL' MARKER_TAG
             * sections will be used.
             *
             * @param filename Path to a geometry file of either .obj or .su2 format.
             */
            MeshGeometry_t(const std::string& filename);

            std::vector<std::string> mat_; /**< @brief Array of strings representing each face's material's name.*/
            std::vector<std::array<Vec3f, 3>> v_; /**< @brief Array of the three points representing each triangular face.*/
            std::vector<std::array<std::array<double, 2>, 3>> vt_; /**< @brief Array of the three uv coordinates representing each triangular face's texture coordinates.*/
            std::vector<std::array<Vec3f, 3>> vn_; /**< @brief Array of the three normals representing each triangular face's normals.*/

        private:
            /**
             * @brief Fills the class' members form a .obj file.
             *
             * @param filename Path to a geometry file in .obj format.
             * @return std::vector<std::array<bool, 3>> Vector indicating which points have missing normals.
             */
            auto readObj(const std::string& filename) -> std::vector<std::array<bool, 3>>;

            /**
             * @brief Fills the class' members form a .su2 file.
             *
             * The faces in the 'WALL' MARKER_TAG sections will be used.
             *
             * @param filename Path to a geometry file in .su2 format.
             * @return std::vector<std::array<bool, 3>> Vector indicating which points have missing normals.
             */
            auto readSU2(const std::string& filename) -> std::vector<std::array<bool, 3>>;

            /**
             * @brief Constructs the face normals from the face points when none are supplied in the file.
             *
             * This will give a facetted look, as the faces will have the same normal at all points.
             *
             * @param normals_to_build Vector indicating which points have missing normals.
             */
            auto build_missing_normals(const std::vector<std::array<bool, 3>>& normals_to_build) -> void;
    };
}}

#endif