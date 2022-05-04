#ifndef APTRACER_ENTITIES_MESHGEOMETRY_T_H
#define APTRACER_ENTITIES_MESHGEOMETRY_T_H

#include "entities/Vec3f.h"
#include <string>
#include <vector>

namespace APTracer { namespace Entities {

    /**
     * @brief The mesh geometry class represents a geometry made up of points and triangular faces.
     * 
     * Mesh geometries represent a single geometry without any transformation. Multiple meshes can point to the same mesh geometry
     * while using different transformations, enabling instanciating and saving ressources. This class is constructed from geometry
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
            MeshGeometry_t(const std::string &filename);

            size_t n_tris_; /**< @brief Number of triangular faces held by the mesh geometry.*/
            std::vector<std::string> mat_; /**< @brief Array of strings representing each face's material's name. Size: n_tris_.*/
            std::vector<Vec3f> v_; /**< @brief Array of points representing the triangular faces. Size: 3*n_tris_. Face i has the points v_[3*i], v_[3*i + 1], v_[3*i + 2].*/
            std::vector<double> vt_; /**< @brief Array of uv coordinates representing the triangular faces' texture coordinates. Size: 6*n_tris_. Face i has the uvs [vt_[6*i], vt_[6*i+1]], [vt_[6*i+2], vt_[6*i+3]], [vt_[6*i+4], vt_[6*i+5]].*/
            std::vector<Vec3f> vn_; /**< @brief Array of normals representing the triangular faces' normals. Size: 3*n_tris_. Face i has the normals vn_[3*i], vn_[3*i + 1], vn_[3*i + 2].*/
        
        private:
            /**
             * @brief Fills the class' members form a .obj file.
             * 
             * @param filename Path to a geometry file in .obj format.
             * @return std::vector<bool> Vector indicating which points have missing normals.
             */
            auto readObj(const std::string &filename) -> std::vector<bool>;

            /**
             * @brief Fills the class' members form a .su2 file.
             * 
             * The faces in the 'WALL' MARKER_TAG sections will be used.
             * 
             * @param filename Path to a geometry file in .su2 format.
             * @return std::vector<bool> Vector indicating which points have missing normals.
             */
            auto readSU2(const std::string &filename) -> std::vector<bool>;

            /**
             * @brief Constructs the face normals from the face points when none are supplied in the file.
             * 
             * This will give a facetted look, as the faces will have the same normal at all points.
             * 
             * @param normals_to_build Vector indicating which points have missing normals.
             */
            auto build_missing_normals(const std::vector<bool>& normals_to_build) -> void;
    };
}}

#endif