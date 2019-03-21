#include "MeshGeometry_t.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

MeshGeometry_t::MeshGeometry_t(const std::string &filename){
    readObj(filename);
    buildTriangles();
}

MeshGeometry_t::~MeshGeometry_t(){
    if (mat_ != nullptr){
        delete [] mat_;
    }

    if (v_ != nullptr){
        delete [] v_;
    }

    if (vn_ != nullptr){
        delete [] vn_;
    }

    if (vt_ != nullptr){
        for (unsigned int i = 0; i < n_tris_; i++){
            if (vt_[i] != nullptr){
                delete [] vt_[i];
            }
        }
        delete [] vt_;
    }

}

void MeshGeometry_t::readObj(const std::string &filename){
    unsigned int nv = 0;
    unsigned int nvt = 0;
    unsigned int nvn = 0;
    unsigned int nf = 0;
    unsigned int nsides;
    std::string line;
    std::string token;
    std::string dummy;

    std::ifstream meshfile(filename);
    if (!meshfile.is_open()) {
        std::cout << "Error: file '" << filename << "' could nor be opened. Exiting." << std::endl;
        return;
    }

    // Getting number of elements
    while (std::getline(meshfile, line)){
        std::istringstream liness(line);
        liness >> token;

        if (token.compare("v")){
            nv++;
        }
        else if (token.compare("vt")){
            nvt++;
        }
        else if (token.compare("vn")){
            nvn++;
        }
        else if (token.compare("f")){
            nsides = 0;
            while (liness >> dummy){
                nsides++;
            }
            nf += nsides - 2;
        }
    }

    // Getting normals, vertex coordinates and texture coordinates
    unsigned int v_counter = 0;
    unsigned int vt_counter = 0;
    unsigned int vn_counter = 0;
    double val0, val1, val2;

    Vec3f* v = new Vec3f[nv];
    double** vt = new double*[nvt];
    for (unsigned int i = 0; i < nvt; i++){
        vt[i] = new double[2];
    }
    Vec3f* vn = new Vec3f[nvn];


    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)){
        std::istringstream liness(line);
        liness >> token;

        if (token.compare("v")){
            liness >> val0 >> val1 >> val2;
            v[v_counter] = Vec3f(val0, val1, val2);
            v_counter++;
        }
        else if (token.compare("vt")){
            liness >> val0 >> val1;
            vt[vt_counter][0] = val0;
            vt[vt_counter][1] = val1;
            vt_counter++;
        }
        else if (token.compare("vn")){
            liness >> val0 >> val1 >> val2;
            vn[vn_counter] = Vec3f(val0, val1, val2);
            vn_counter++;
        }
    }    

    // Filling faces
    unsigned int nf_counter = 0;
    std::string material = "";
    std::string token1, token2;

    n_tris_ = nf;
    mat_ = new std::string[n_tris_];
    v_ = new Vec3f[n_tris_];
    vt_ = new double*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        vt_[i] = new double[2];
    }
    vn_ = new Vec3f[n_tris_];

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)){
        std::istringstream liness(line);
        liness >> token;

        if (token.compare("f")){
            liness >> token >> token1;
            while (liness >> token2){


                token1 = token2;
            }
            
        }
        else if (token.compare("usemtl")){ // check if there is :
            
        }
    }    

    meshfile.close();


    delete [] v;
    for (unsigned int i = 0; i < nvt; i++){
        delete [] vt[i];
    }
    delete [] vt;
    delete [] vn;
}

void MeshGeometry_t::buildTriangles(){
    
}