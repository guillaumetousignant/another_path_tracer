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
#include "ImgBufferOpenGL_t.h"
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
#include <cmath>
#include <chrono>

#include "GL/glut.h"
#include "GL/gl.h"

#include <iostream> // REMOVE

#define PI 3.141592653589793238463
#ifdef _WIN32
#define GL_CLAMP_TO_EDGE 0x812F
#endif

Cam_t* thecamera;
Scene_t* thescene;
double width, height;
int right_x_pos = 0;
int right_y_pos = 0;
bool right_clicked = false;
int n_iter_gl = 0;

void raytrace(){
    n_iter_gl++;
    //auto t_start = std::chrono::high_resolution_clock::now();
    thecamera->raytrace(thescene);
    //auto t_end = std::chrono::high_resolution_clock::now();

    /*std::cout << "Iteration " << n_iter_gl << " done in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl;*/
    glutPostRedisplay(); // REMOVE but makes it work, soooooooo...
}

void resetDisplay(void){
    thecamera->reset();
    n_iter_gl = 0;
}

void mouse_movement(int x, int y){
    if (right_clicked){
        double differential_x = double(x - right_x_pos)/double(width);
        double differential_y = double(y - right_y_pos)/double(height);
        right_x_pos = x;
        right_y_pos = y;

        thecamera->transformation_->rotateY(differential_x/thecamera->fov_[1]); // CHECK those should be switched
        thecamera->transformation_->rotateX(differential_y/thecamera->fov_[0]); // CHECK those should be switched
        thecamera->update();
        n_iter_gl = 0;
    }
}

void mouse_click(int button, int state, int x, int y){
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
        {
            glutIdleFunc(nullptr);
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        if(state == GLUT_DOWN)
        {
            resetDisplay();
        }
        break;

    case GLUT_RIGHT_BUTTON:
        if(state == GLUT_DOWN)
        {            
            right_clicked = true;
            right_x_pos = x;
            right_y_pos = y;
        }

        else if(state == GLUT_UP){
            right_clicked = false;
        }
        break;
    }
}

void keyboard(unsigned char key, int x, int y){
    auto t_start_write = std::chrono::high_resolution_clock::now(); // why is this needed?
    auto t_end_write = t_start_write;
    double position[2];
    switch (key)
    {
    case 's':
        std::cout << "Writing started." << std::endl;
        t_start_write = std::chrono::high_resolution_clock::now();
        thecamera->write();
        t_end_write = std::chrono::high_resolution_clock::now();

        std::cout << "Writing done in "
        << std::chrono::duration<double, std::milli>(t_end_write-t_start_write).count()/1000.0 
        << "s." << std::endl;
        break;

    case 'f':
        position[0] = std::min(std::max(double(x)/double(width), 0.0), 1.0);
        position[1] = std::min(std::max(double(y)/double(height), 0.0), 1.0);
        thecamera->autoFocus(thescene, position);
        break;

    case 'q':
        glutDestroyWindow(0);
        exit(0);
        break;
    }
}

int main(int argc, char **argv){
    auto t_start = std::chrono::high_resolution_clock::now();
    Texture_t* zombietex = new Texture_t("./assets/Zombie beast_texture5.png");
    //Texture_t* pipertex = new Texture_t("./assets/piper_pa18_obj/piper_diffuse.png");

    MeshGeometry_t* cubemesh = new MeshGeometry_t("./assets/cube.obj");
    MeshGeometry_t* zombiemesh = new MeshGeometry_t("./assets/Zombie_Beast4_test2.obj");
    //MeshGeometry_t* pipermesh = new MeshGeometry_t("./assets/piper_pa18_obj/piper_pa18.obj");

    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    Absorber_t* glassabsorber = new Absorber_t(Vec3f(), Vec3f(0.6, 0.95, 0.8), 100, 2.0);

    Transparent_t* air = new Transparent_t(0, airabsorber);
    Diffuse_t* difpurple = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 0.7, 0.85), 1);
    Diffuse_t* diflight = new Diffuse_t(Vec3f(2.0, 2.0, 2.0), Vec3f(1.0, 1.0, 1.0), 1);
    Diffuse_t* difgreen = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.8, 0.95, 0.6), 1);
    Diffuse_t* difblue = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.1, 0.4, 0.8), 1);
    DiffuseTex_t* zombiemat = new DiffuseTex_t(Vec3f(0.0, 0.0, 0.0), zombietex, 0.2);
    //DiffuseTex_t* pipermat = new DiffuseTex_t(Vec3f(0.0, 0.0, 0.0), pipertex, 0.8);
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
    //TransformMatrix_t* transform_piper = new TransformMatrix_t();
    TransformMatrix_t* transform_cube = new TransformMatrix_t();
    TransformMatrix_t* transform_neutral = new TransformMatrix_t();

    Sphere_t* spherepurple = new Sphere_t(difpurple, transform1);
    Sphere_t* mirror = new Sphere_t(ref1, transform2);
    Sphere_t* light = new Sphere_t(diflight, transform3);
    Sphere_t* sphereglass = new Sphere_t(glass, transform4);
    Sphere_t* ground = new Sphere_t(difgreen, transform5);

    Vec3f points1[3];
    Vec3f points2[3];

    points1[0] = Vec3f(-2, 4, -0.5);
    points1[1] = Vec3f(-2, -4, -0.5);
    points1[2] = Vec3f(2, -4, -0.5);
    
    points2[0] = Vec3f(-2, 4, -0.5);
    points2[1] = Vec3f(2, -4, -0.5);
    points2[2] = Vec3f(2, 4, -0.5);

    Triangle_t* planegrey1 = new Triangle_t(difgreen, transform_neutral, &points1[0], nullptr, nullptr);
    Triangle_t* planegrey2 = new Triangle_t(difgreen, transform_neutral, &points2[0], nullptr, nullptr);

    Mesh_t* cube = new Mesh_t(difblue, transform_cube, cubemesh);
    Mesh_t* zombie = new Mesh_t(zombiemat, transform_zombie, zombiemesh);
    //Mesh_t* piper = new Mesh_t(pipermat, transform_piper, pipermesh);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements created in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    spherepurple->transformation_->translate(Vec3f(1, 2, 0.5));
    spherepurple->transformation_->scale(0.5);
    mirror->transformation_->translate(Vec3f(-1.5, 4, -0.8));
    mirror->transformation_->scale(1.5);
    light->transformation_->translate(Vec3f(0, 3, 0.8));
    light->transformation_->scale(0.75);
    sphereglass->transformation_->translate(Vec3f(0.5, 2.0, 0.2));
    sphereglass->transformation_->scale(0.4);
    ground->transformation_->translate(Vec3f(0, 0, -65));
    ground->transformation_->scale(64);
    zombie->transformation_->translate(Vec3f(0.0, 2.0, -0.53));
    zombie->transformation_->scale(0.025);
    zombie->transformation_->rotateX(PI/2);
    zombie->transformation_->rotateZ(-PI/16);
    cube->transformation_->translate(Vec3f(0.5, 2.0, 0.2));
    cube->transformation_->scale(0.5);
    cube->transformation_->rotateX(0);
    cube->transformation_->rotateZ(PI/8);
    /*piper->transformation_->translate(Vec3f(0.0, 1.75, -0.25));
    piper->transformation_->scale(0.2);
    piper->transformation_->rotateX(PI/2.0);
    piper->transformation_->rotateZ(PI/8.0);*/
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements transformed in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    Scene_t* scene = new Scene_t();
    thescene = scene;
    //scene->add(spherepurple);
    //scene->add(mirror);
    //scene->add(light);
    //scene->add(sphereglass);
    //scene->add(ground);
    scene->add(zombie);
    scene->add(planegrey1);
    scene->add(planegrey2);
    //scene->add(piper);
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements added in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    scene->update();
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene updated in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    scene->build_acc();
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Acceleration structure built in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl << std::endl;

    // Camera stuff

    std::string filename = "./images/test.png";

    unsigned int res_x = 300;
    unsigned int res_y = 200;
    width = res_x;
    height = res_y;
    double fov[2];
    fov[1] = 80.0 * PI /180.0; 
    fov[0] = fov[1] * res_y/res_x;
    unsigned int subpix[2] = {1, 1};
    unsigned int maxbounces = 16;

    DirectionalLight_t* dirlight = new DirectionalLight_t(Vec3f(5, 5, 4), transform_light);
    dirlight->transformation_->scale(0.95);
    dirlight->transformation_->rotateZ(-0.7854);
    dirlight->transformation_->rotateX(-1.1781);
    dirlight->update();
    
    SkyboxFlatSun_t* skybox = new SkyboxFlatSun_t(Vec3f(0.85, 0.85, 0.98), dirlight);
    ImgBufferOpenGL_t* imgbuffer = new ImgBufferOpenGL_t(res_x, res_y);

    std::list<Medium_t*> medium_list;
    medium_list.assign(2, air);

    Cam_t* cam = new Cam_t(transform_camera, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, medium_list, skybox, maxbounces, 1.0);
    thecamera = cam;
    cam->update();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(res_x, res_y);
    glutInitWindowPosition(10,10);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(raytrace);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_movement);
    glutKeyboardFunc(keyboard);

    glGenTextures( 1, &(imgbuffer->tex_) );
    glBindTexture( GL_TEXTURE_2D, imgbuffer->tex_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, res_x, res_y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );    

    glutMainLoop();

    std::cout << "Don't get here" << std::endl; // REMOVE
    //cam->accumulateWrite(scene, 10000, 100);
    //cam->write();

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