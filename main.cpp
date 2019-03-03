#include "RandomGenerator_t.h"
#include "Diffuse_t.h"
#include "Vec3f.h"
#include "Sphere_t.h"
#include "TransformMatrix_t.h"
#include "Scene_t.h"
#include "SkyboxFlat_t.h"
#include "ImgBuffer_t.h"
#include "Cam_t.h"
#include <string>
#include <list>
#include "NonAbsorber_t.h"
#include "Transparent_t.h"
#include <string>

#include <iostream> // REMOVE

int main(){
    Diffuse_t* diffuse = new Diffuse_t(Vec3f(0, 0, 0), Vec3f(0.5, 0.5, 0.5), 0.5);
    TransformMatrix_t* transform = new TransformMatrix_t();
    Sphere_t* sphere = new Sphere_t(diffuse, transform);
    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    Transparent_t* air = new Transparent_t(0, airabsorber);

    Scene_t* scene = new Scene_t();
    scene->add(sphere);

    SkyboxFlat_t* skybox = new SkyboxFlat_t(Vec3f(1.0, 1.0, 1.0));
    ImgBuffer_t* imgbuffer = new ImgBuffer_t(300, 200);

    std::list<Medium_t*> medium_list;
    medium_list.push_back(air);
    medium_list.push_back(air);

    std::string filename = "test.png";

    double fov[2] ={60, 40}; 
    unsigned int subpix[2] = {1, 1};
    unsigned int maxbounces = 8;

    Cam_t* cam = new Cam_t(transform, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, medium_list, skybox, maxbounces, 1.0);

    cam->accumulate(scene, 10);

    cam->write();

    return 0;
}