#include "MaterialMap_t.h"
#include "Material_t.h"
#include <map>

MaterialMap_t::MaterialMap_t(std::string* names, Material_t** materials, unsigned int n) {
    for (unsigned int i = 0; i < n; i++){
        materials_[names[i]] = materials[i];
    }
}

MaterialMap_t::~MaterialMap_t(){}

Material_t* MaterialMap_t::getMaterial(const std::string &key) const {
    return materials_.at(key);
}

Material_t* MaterialMap_t::getFirst() const {
    return materials_.begin()->second;
}