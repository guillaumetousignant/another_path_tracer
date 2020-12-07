#ifndef APTRACER_SLERP_H
#define APTRACER_SLERP_H

namespace APTracer {
    /**
     * @brief Circular coordinates interpolation. Can interpolate angles in radians.
     * 
     * The angles should be lower than 4π.
     * 
     * @param phi_1 First angle in radians.
     * @param phi_2 Second angle in radians.
     * @param t Interpolation point between the two angles. Between 0 and 1, 0 gives phi_1 and 1 gives phi_2.
     * @return double Interpolated angle.
     */
    double slerp(double phi_1, double phi_2, double t);
}

#endif