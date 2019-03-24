#include "RandomGenerator_t.h"
#include "Diffuse_t.h"
#include "DiffuseTex_t.h"
#include "Refractive_t.h"
#include "Reflective_t.h"
#include "ReflectiveRefractiveFuzz_t.h"
#include "ReflectiveRefractive_t.h"
#include "Vec3f.h"
#include "Sphere_t.h"
#include "Triangle_t.h"
#include "Mesh_t.h"
#include "TransformMatrix_t.h"
#include "Scene_t.h"
//#include "SkyboxFlat_t.h"
#include "ImgBuffer_t.h"
#include "Cam_t.h"
#include <string>
#include <list>
#include "NonAbsorber_t.h"
#include "Absorber_t.h"
#include "Transparent_t.h"
#include <string>
#include "DirectionalLight_t.h"
#include "Texture_t.h"
#include "MeshGeometry_t.h"
#include "SkyboxFlatSun_t.h"

#include <iostream> // REMOVE

#define PI 3.141592653589793238463

int main(){
    Texture_t* zombietex = new Texture_t("./assets/Zombie beast_texture5.png");
    MeshGeometry_t* zombiemesh = new MeshGeometry_t("./assets/Zombie_Beast4_test2.obj");

    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    Absorber_t* glassabsorber = new Absorber_t(Vec3f(), Vec3f(0.6, 0.95, 0.8), 100, 2.0);

    Transparent_t* air = new Transparent_t(0, airabsorber);
    Diffuse_t* difpurple = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 0.7, 0.85), 1);
    Diffuse_t* diflight = new Diffuse_t(Vec3f(2.0, 2.0, 2.0), Vec3f(1.0, 1.0, 1.0), 1);
    Diffuse_t* difgreen = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.8, 0.95, 0.6), 1);
    Diffuse_t* difblue = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.1, 0.4, 0.8), 1);
    DiffuseTex_t* zombiemat = new DiffuseTex_t(Vec3f(0.0, 0.0, 0.0), zombietex, 0.2);
    Reflective_t* ref1 = new Reflective_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 1, 0.9));
    //ReflectiveRefractiveFuzz_t* glass = new ReflectiveRefractiveFuzz_t(Vec3f(0.0, 0.0, 0.0), Vec3f(1.0, 1.0, 1.0), 1.5, 10, 1.0, 0.1, glassabsorber);
    Refractive_t* glass = new Refractive_t(Vec3f(0.0, 0.0, 0.0), Vec3f(1.0, 1.0, 1.0), 1.5, 10, glassabsorber);
    TransformMatrix_t* transform_light = new TransformMatrix_t();
    TransformMatrix_t* transform_camera = new TransformMatrix_t(); 
    TransformMatrix_t* transform1 = new TransformMatrix_t();
    TransformMatrix_t* transform2 = new TransformMatrix_t();
    TransformMatrix_t* transform3 = new TransformMatrix_t();
    TransformMatrix_t* transform4 = new TransformMatrix_t();
    TransformMatrix_t* transform5 = new TransformMatrix_t();
    TransformMatrix_t* transform_zombie = new TransformMatrix_t();

    Sphere_t* spherepurple = new Sphere_t(difpurple, transform1);
    Sphere_t* mirror = new Sphere_t(ref1, transform2);
    Sphere_t* light = new Sphere_t(diflight, transform3);
    Sphere_t* sphereglass = new Sphere_t(glass, transform4);
    Sphere_t* ground = new Sphere_t(difgreen, transform5);

    Mesh_t* zombie = new Mesh_t(zombiemat, transform_zombie, zombiemesh);

    spherepurple->transformation_->translate(Vec3f(1, 2, 0.5));
    spherepurple->transformation_->scale(0.5);
    mirror->transformation_->translate(Vec3f(-1.5, 4, -0.8));
    mirror->transformation_->scale(1.5);
    light->transformation_->translate(Vec3f(0, 3, 0.8));
    light->transformation_->scale(0.75);
    sphereglass->transformation_->translate(Vec3f(0.5, 2.0, 0.2));
    sphereglass->transformation_->scale(0.4);
    ground->transformation_->translate(Vec3f(0, 0, -101));
    ground->transformation_->scale(100);
    zombie->transformation_->translate(Vec3f(0.0, 2.0, -0.53));
    zombie->transformation_->scale(0.025);
    zombie->transformation_->rotateX(PI/2);
    zombie->transformation_->rotateZ(-PI/16);

    Scene_t* scene = new Scene_t();
    //scene->add(spherepurple);
    //scene->add(mirror);
    //scene->add(light);
    //scene->add(sphereglass);
    scene->add(ground);
    scene->add(zombie);
    scene->update();

    scene->build_acc();

    DirectionalLight_t* dirlight = new DirectionalLight_t(Vec3f(5, 5, 4), transform_light);
    dirlight->transformation_->scale(0.95);
    dirlight->transformation_->rotateZ(-0.7854);
    dirlight->transformation_->rotateX(-1.1781);
    dirlight->update();
    
    SkyboxFlatSun_t* skybox = new SkyboxFlatSun_t(Vec3f(0.85, 0.85, 0.98), dirlight);
    ImgBuffer_t* imgbuffer = new ImgBuffer_t(1800, 1200);

    std::list<Medium_t*> medium_list;
    medium_list.assign(2, air);

    std::string filename = "./images/test.png";

    double fov[2];
    fov[1] = 80.0 * PI /180.0; 
    fov[0] = fov[1] * imgbuffer->size_y_/imgbuffer->size_x_;
    unsigned int subpix[2] = {1, 1};
    unsigned int maxbounces = 16;

    Cam_t* cam = new Cam_t(transform_camera, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, medium_list, skybox, maxbounces, 1.0);
    cam->update();

    cam->accumulateWrite(scene, 10000, 10);
    cam->write();

    /*for (unsigned int i = 0; i < 1; i++){
        Ray_t ray = Ray_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list);
        ray.raycast(scene, 8, skybox);
    }*/
    
    /*std::cout << "Left: " << std::endl; // REMOVE
    Ray_t rayL = Ray_t(Vec3f(0.0, 0.0, 0.0), Vec3f(-0.447643, 0.894212, -0.000745399), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list);
    rayL.raycast(scene, 2, skybox);

    std::cout << std::endl << "Right: " << std::endl; // REMOVE
    Ray_t rayR = Ray_t(Vec3f(0.0, 0.0, 0.0), Vec3f(-0.435534, 0.900172, -0.000281008), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list);
    rayR.raycast(scene, 2, skybox); // These are never given a downward direction, never intersect with ground
    */
    return 0;
}