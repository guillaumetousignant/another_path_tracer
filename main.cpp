#include "RandomGenerator_t.h"
#include "Diffuse_t.h"
#include "Vec3f.h"
#include "Sphere_t.h"
#include "TransformMatrix_t.h"
#include "Scene_t.h"
//#include "SkyboxFlat_t.h"
#include "ImgBuffer_t.h"
#include "Cam_t.h"
#include <string>
#include <list>
#include "NonAbsorber_t.h"
#include "Transparent_t.h"
#include <string>
#include "DirectionalLight_t.h"
#include "SkyboxFlatSun_t.h"

//#include <iostream> // REMOVE

#define PI 3.141592653589793238463

int main(){
    Diffuse_t* diffuse = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 0.5);
    TransformMatrix_t* transform = new TransformMatrix_t();
    Sphere_t* sphere = new Sphere_t(diffuse, transform);
    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    Transparent_t* air = new Transparent_t(0, airabsorber);

    Scene_t* scene = new Scene_t();
    scene->add(sphere);

    DirectionalLight_t* dirlight = new DirectionalLight_t(Vec3f(8.0, 8.0, 6.0), transform);
    SkyboxFlatSun_t* skybox = new SkyboxFlatSun_t(Vec3f(0.85, 0.85, 0.98), dirlight);
    ImgBuffer_t* imgbuffer = new ImgBuffer_t(1800, 1200);

    std::list<Medium_t*> medium_list;
    medium_list.push_back(air);
    medium_list.push_back(air);

    std::string filename = "./images/test.png";

    double fov[2] ={40.0 * 2.0 * PI/360.0, 60.0 * 2.0 * PI /360.0}; 
    unsigned int subpix[2] = {1, 1};
    unsigned int maxbounces = 8;

    Cam_t* cam = new Cam_t(transform, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, medium_list, skybox, maxbounces, 1.0);

    cam->accumulate(scene, 100);
    cam->write();

    /*for (unsigned int i = 0; i < 1; i++){
        Ray_t ray = Ray_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list);
        ray.raycast(scene, 8, skybox);
    }*/

    return 0;
}