#include "entities/MaterialMap_t.h"
#include "entities/Material_t.h"
#include <map>

using APTracer::Entities::MaterialMap_t;
using APTracer::Entities::Material_t;

MaterialMap_t::MaterialMap_t(std::string* names, Material_t** materials, size_t n) {
    for (size_t i = 0; i < n; i++) {
        materials_[names[i]] = materials[i];
    }
}

MaterialMap_t::~MaterialMap_t() = default;

auto MaterialMap_t::getMaterial(const std::string &key) const -> Material_t* {
    return materials_.at(key);
}

auto MaterialMap_t::getFirst() const -> Material_t* {
    return materials_.begin()->second;
}