#ifndef MATERIALMAP_T_H
#define MATERIALMAP_T_H

#include <string>
#include <map>

class Material_t;

class MaterialMap_t{
    public:
        MaterialMap_t(std::string* names, Material_t** materials, unsigned int n);
        ~MaterialMap_t();

        std::map<std::string, Material_t*> materials_;

        Material_t* getMaterial(const std::string &key) const;
};

#endif