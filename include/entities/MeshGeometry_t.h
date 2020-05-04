#ifndef MESHGEOMETRY_T_H
#define MESHGEOMETRY_T_H

#include <string>
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {

    /**
     * @brief The mesh geometry class represents a geometry made up of points and triangular faces.
     * 
     * Mesh geometries represent a single geometry without any transformation. Multiple meshes can point to the same mesh geometry
     * while using different transformations, enabling instanciating and saving ressources. This class is constructed from geometry
     * input files. Currently, .obj and .su2 files are supported.
     */
    class MeshGeometry_t{
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

            /**
             * @brief Destroy the MeshGeometry_t object, freeing the memory ressources held by it.
             */
            ~MeshGeometry_t();

            unsigned int n_tris_; /**< @brief Number of triangular faces held by the mesh geometry.*/
            std::string* mat_; /**< @brief Array of strings representing each face's material's name. Size: n_tris_.*/
            Vec3f* v_; /**< @brief Array of points representing the triangular faces. Size: 3*n_tris_. Face i has the points v_[3*i], v_[3*i + 1], v_[3*i + 2].*/
            double* vt_; // Is this legal?
            Vec3f* vn_;
        
        private:
            void readObj(const std::string &filename);
            void readSU2(const std::string &filename);
            void deNan();
    };
}}

#endif