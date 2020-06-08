#ifndef MATERIALS_H
#define MATERIALS_H

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
    namespace Materials {}
}

#include "Absorber_t.h"
#include "BounceMaterial_t.h"
#include "Diffuse_t.h"
#include "DiffuseFull_t.h"
#include "DiffuseNormal_t.h"
#include "DiffuseTex_t.h"
#include "DiffuseTexNormal_t.h"
#include "DistanceMaterial_t.h"
#include "FresnelMix_t.h"
#include "FresnelMixIn_t.h"
#include "FresnelMixNormal_t.h"
#include "NonAbsorber_t.h"
#include "NormalMaterial_t.h"
#include "NormalDiffuseMaterial_t.h"
#include "Portal_t.h"
#include "PortalScatterer_t.h"
#include "PortalScattererTop_t.h"
#include "PortalTop_t.h"
#include "RandomMix_t.h"
#include "RandomMixIn_t.h"
#include "Reflective_t.h"
#include "ReflectiveFuzz_t.h"
#include "ReflectiveFuzzNormal_t.h"
#include "ReflectiveFuzzTex_t.h"
#include "ReflectiveFuzzTexNormal_t.h"
#include "ReflectiveNormal_t.h"
#include "ReflectiveRefractive_t.h"
#include "ReflectiveRefractiveFuzz_t.h"
#include "ReflectiveRefractiveNormal_t.h"
#include "Refractive_t.h"
#include "RefractiveFuzz_t.h"
#include "Scatterer_t.h"
#include "ScattererExp_t.h"
#include "ScattererExpFull_t.h"
#include "ScattererFull_t.h"
#include "Toon_t.h"
#include "Transparent_t.h"

#endif