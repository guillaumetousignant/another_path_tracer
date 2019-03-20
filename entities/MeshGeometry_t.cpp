#include "MeshGeometry_t.h"

MeshGeometry_t::MeshGeometry_t(){

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