#ifndef MATERIALMAP_T_H
#define MATERIALMAP_T_H

#include <string>
#include <map>

namespace APTracer { namespace Entities {
    class Material_t;

    /**
     * @brief The material map class holds materials and their names, to find referenced materials in, forr example, obj files.
     * 
     * This is the class used to initialise mesh geometries with multiple materials.
     */
    class MaterialMap_t{
        public:
            /**
             * @brief Construct a new MaterialMap_t object
             * 
             * @param names Array of strings representing the names of the materials, in order. Size: n.
             * @param materials Array of pointers to the materials represented by the names, in order. Size: n.
             * @param n Size of both input arrays.
             */
            MaterialMap_t(std::string* names, Material_t** materials, unsigned int n);

            /**
             * @brief Destroy the MaterialMap_t object. Does nothing.
             */
            ~MaterialMap_t();

            std::map<std::string, Material_t*> materials_; /**< @brief Map of material pointers using string as keys. The keys represent the materials' names.*/

            /**
             * @brief Get the material pointer associated with a specific name.
             * 
             * No checks are made, will throw out_of_range if the key is not present.
             * 
             * @param key String representing the material's name.
             * @return Material_t* Pointer to the material which has the input name.
             */
            Material_t* getMaterial(const std::string &key) const;

            /**
             * @brief Get the first material pointer from the map.
             * 
             * Useful when an object must have one material only, even if it uses multiple materials internally. Was the case for old meshes, which were shapes.
             * 
             * @return Material_t* Pointer to the first material of the map.
             */
            Material_t* getFirst() const;
    };
}}

#endif