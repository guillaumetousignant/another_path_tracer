#include "PortalScattererTop_t.h"
#include "TransformMatrix_t.h"
#include "Medium_t.h"

PortalScattererTop_t::PortalScattererTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list)
    : transformation_(transformation), medium_list_(medium_list) {}

PortalScattererTop_t::~PortalScattererTop_t() {}