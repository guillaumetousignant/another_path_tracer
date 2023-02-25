#include "entities/MaterialMap_t.hpp"

using APTracer::Entities::MaterialMap_t;

MaterialMap_t::MaterialMap_t(std::vector<std::string> names, std::vector<Material_t*> materials) {
    for (size_t i = 0; i < names.size(); i++) {
        materials_[names[i]] = materials[i];
    }
}

auto MaterialMap_t::getMaterial(const std::string& key) const -> Material_t* {
    return materials_.at(key);
}

auto MaterialMap_t::getFirst() const -> Material_t* {
    return materials_.begin()->second;
}