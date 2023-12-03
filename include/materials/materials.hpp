#ifndef APTRACER_MATERIALS_MATERIALS_HPP
#define APTRACER_MATERIALS_MATERIALS_HPP

namespace APTracer {
    /**
     * @brief Contains different material and medium types that can be used.
     *
     * Materials and mediums describes how some entities interact with light.
     * Materials describe how shapes interact with light when they are intersected by a ray.
     * They can change the ray's colour and mask values, and the ray's origin and direction.
     * Mediums describe how light interacts when being inside a medium. They can change the
     * ray's colour and mask relatively to the distance traveled through the medium. They
     * can also intersect the ray themselves, changing the ray's colour and mask, and its
     * origin and direction. In that case, it reports that it has intersected the ray so
     * the shape intersection can be ignored.
     */
    namespace Materials {
    }
}

#include "Absorber_t.hpp"
#include "BounceMaterial_t.hpp"
#include "Diffuse_t.hpp"
#include "DiffuseFull_t.hpp"
#include "DiffuseNormal_t.hpp"
#include "DiffuseTex_t.hpp"
#include "DiffuseTexNormal_t.hpp"
#include "DiffuseTexNormalRough_t.hpp"
#include "DistanceMaterial_t.hpp"
#include "FresnelMix_t.hpp"
#include "FresnelMixIn_t.hpp"
#include "FresnelMixNormal_t.hpp"
#include "NonAbsorber_t.hpp"
#include "NormalDiffuseMaterial_t.hpp"
#include "NormalMaterial_t.hpp"
#include "Portal_t.hpp"
#include "PortalScatterer_t.hpp"
#include "RandomMix_t.hpp"
#include "RandomMixIn_t.hpp"
#include "Reflective_t.hpp"
#include "ReflectiveFuzz_t.hpp"
#include "ReflectiveFuzzNormal_t.hpp"
#include "ReflectiveFuzzTex_t.hpp"
#include "ReflectiveFuzzTexNormal_t.hpp"
#include "ReflectiveNormal_t.hpp"
#include "ReflectiveRefractive_t.hpp"
#include "ReflectiveRefractiveFuzz_t.hpp"
#include "ReflectiveRefractiveNormal_t.hpp"
#include "Refractive_t.hpp"
#include "RefractiveFuzz_t.hpp"
#include "Scatterer_t.hpp"
#include "ScattererExp_t.hpp"
#include "ScattererExpFull_t.hpp"
#include "ScattererFull_t.hpp"
#include "TextureMix_t.hpp"
#include "TextureMixIn_t.hpp"
#include "Toon_t.hpp"
#include "Transparent_t.hpp"

#endif
