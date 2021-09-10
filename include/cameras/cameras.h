#ifndef APTRACER_CAMERAS_H
#define APTRACER_CAMERAS_H

namespace APTracer { 
    /**
     * @brief Contains the different camera types that can be used.
     * 
     * Cameras create rays according to a projection, and casts them in a scene. Then, the
     * colour of the rays are usually stored in an image buffer. The different cameras have
     * different projections or effects, such as motion blur, depth of field and 3D.
     */
    namespace Cameras {}
}

#include "Cam_t.h"
#include "Cam3D_t.h"
#include "Cam3DAperture_t.h"
#include "Cam3DMotionblur_t.h"
#include "Cam3DMotionblurAperture_t.h"
#include "CamAperture_t.h"
#include "CamMotionblur_t.h"
#include "CamMotionblurAperture_t.h"
#include "FishCam_t.h"
#include "FishCamAperture_t.h"
#include "FishCamMotionblur_t.h"
#include "FishCamMotionblurAperture_t.h"
#include "IsoCam_t.h"
#include "IsoCamAperture_t.h"
#include "IsoCamMotionblur_t.h"
#include "IsoCamMotionblurAperture_t.h"
#include "RecCam_t.h"
#include "RecCamAperture_t.h"
#include "RecCamMotionblur_t.h"
#include "RecCamMotionblurAperture_t.h"

#endif