#ifndef APTRACER_CAMERAS_CAMERAS_HPP
#define APTRACER_CAMERAS_CAMERAS_HPP

namespace APTracer {
    /**
     * @brief Contains the different camera types that can be used.
     *
     * Cameras create rays according to a projection, and casts them in a scene. Then, the
     * colour of the rays are usually stored in an image buffer. The different cameras have
     * different projections or effects, such as motion blur, depth of field and 3D.
     */
    namespace Cameras {
    }
}

#include "Cam3D_t.hpp"
#include "Cam3DAperture_t.hpp"
#include "Cam3DMotionblur_t.hpp"
#include "Cam3DMotionblurAperture_t.hpp"
#include "Cam_t.hpp"
#include "CamAperture_t.hpp"
#include "CamMotionblur_t.hpp"
#include "CamMotionblurAperture_t.hpp"
#include "FishCam_t.hpp"
#include "FishCamAperture_t.hpp"
#include "FishCamMotionblur_t.hpp"
#include "FishCamMotionblurAperture_t.hpp"
#include "IsoCam_t.hpp"
#include "IsoCamAperture_t.hpp"
#include "IsoCamMotionblur_t.hpp"
#include "IsoCamMotionblurAperture_t.hpp"
#include "RecCam_t.hpp"
#include "RecCamAperture_t.hpp"
#include "RecCamMotionblur_t.hpp"
#include "RecCamMotionblurAperture_t.hpp"

#endif