#include "MeshGeometry_t.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

MeshGeometry_t::MeshGeometry_t(const std::string &filename){
    readObj(filename);
    deNan();
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
        for (unsigned int i = 0; i < 3*n_tris_; i++){
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

        if (token == "v"){
            nv++;
        }
        else if (token == "vt"){
            nvt++;
        }
        else if (token == "vn"){
            nvn++;
        }
        else if (token == "f"){
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

        if (token == "v"){
            liness >> val0 >> val1 >> val2;
            v[v_counter] = Vec3f(val0, val1, val2);
            v_counter++;
        }
        else if (token == "vt"){
            liness >> val0 >> val1;
            vt[vt_counter][0] = val0;
            vt[vt_counter][1] = val1;
            vt_counter++;
        }
        else if (token == "vn"){
            liness >> val0 >> val1 >> val2;
            vn[vn_counter] = Vec3f(val0, val1, val2);
            vn_counter++;
        }
    }   

    // Filling faces
    unsigned int f_counter = 0;
    std::string material = "";
    std::string tokens[3];
    std::string value;
    size_t pos;

    n_tris_ = nf;
    mat_ = new std::string[n_tris_];
    v_ = new Vec3f[3*n_tris_];
    vt_ = new double*[3*n_tris_];
    for (unsigned int i = 0; i < 3*n_tris_; i++){
        vt_[i] = new double[2];
    }
    vn_ = new Vec3f[3*n_tris_];

    meshfile.clear();
    meshfile.seekg(0, std::ios::beg);

    while (std::getline(meshfile, line)){
        std::istringstream liness(line);
        liness >> token;
        if (token == "f"){
            liness >> tokens[0] >> tokens[1];
            while (liness >> tokens[2]){
                for (unsigned int i = 0; i < 3; i++){
                    mat_[f_counter] = material;
                    pos = tokens[i].find("/");
                    if (pos == std::string::npos){
                        v_[f_counter*3 + i] = v[std::stoi(tokens[i], nullptr)-1];
                        vt_[f_counter*3 + i][0] = 0;
                        vt_[f_counter*3 + i][1] = 0;
                        vn_[f_counter*3 + i] = Vec3f(NAN, NAN, NAN);
                    }
                    else{
                        v_[f_counter*3 + i] = v[std::stoi(tokens[i].substr(0, pos), nullptr)-1];
                        tokens[i].erase(0, pos + 1);

                        pos = tokens[i].find("/");
                        if (pos == std::string::npos){
                            vt_[f_counter*3 + i][0] = vt[std::stoi(tokens[i], nullptr)-1][0];
                            vt_[f_counter*3 + i][1] = vt[std::stoi(tokens[i], nullptr)-1][1];
                            vn_[f_counter*3 + i] = Vec3f(NAN, NAN, NAN);
                        }
                        else{
                            if (pos == 0){
                                vt_[f_counter*3 + i][0] = 0;
                                vt_[f_counter*3 + i][1] = 0; 
                            }
                            else{
                                vt_[f_counter*3 + i][0] = vt[std::stoi(tokens[i].substr(0, pos), nullptr)-1][0];
                                vt_[f_counter*3 + i][1] = vt[std::stoi(tokens[i].substr(0, pos), nullptr)-1][1];
                            }
                            tokens[i].erase(0, pos + 1);
                            vn_[f_counter*3 + i] = vn[std::stoi(tokens[i], nullptr)-1];
                        }
                    }
                }
                f_counter++;
                tokens[1] = tokens[2];
            }
            
        }
        else if (token == "usemtl"){ // check if there is :
            liness >> tokens[0];
            pos = tokens[0].find(":");
            if (pos != std::string::npos){
                tokens[0].erase(0, pos + 1);
            }
            material = tokens[0];
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

void MeshGeometry_t::deNan(){
    for (unsigned int i = 0; i < n_tris_; i++){
        for (unsigned int j = 0; j < 3; j++){
            if (std::isnan(vn_[3*i + j][0])){ // Just checking first value, maybe add isnan to vec3f class?
                vn_[3*i + j] = (v_[3*i + 1] - v_[3*i]).cross(v_[3*i + 2] - v_[3*i]).normalize();
            }
        }
    }
}