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
Mesh_t* themesh;
double width, height;
int right_x_pos = 0;
int right_y_pos = 0;
int left_x_pos = 0;
int left_y_pos = 0;
bool right_clicked = false;
bool left_clicked = false;
int n_iter_gl = 0;
Vec3f focus_point;
double camera_dist = 5;

void raytrace(){
    n_iter_gl++;
    auto t_start = std::chrono::high_resolution_clock::now();
    thecamera->raytrace(thescene);
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Iteration " << n_iter_gl << " done in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl;
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

        themesh->transformation_->rotateY(differential_x/thecamera->fov_[1]); // CHECK those should be switched
        themesh->transformation_->rotateX(differential_y/thecamera->fov_[0]); // CHECK those should be switched
        themesh->update();
        thescene->build_acc();
        resetDisplay();
        n_iter_gl = 0;
    }
    if (left_clicked){
        //double differential_x = double(x - left_x_pos)/double(width);
        double differential_y = double(y - left_y_pos)/double(height);
        left_x_pos = x;
        left_y_pos = y;

        themesh->transformation_->scale(std::pow(2.0, differential_y)); // CHECK those should be switched
        themesh->update();
        thescene->build_acc();
        resetDisplay();
        n_iter_gl = 0;
    }
}

void mouse_click(int button, int state, int x, int y){
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
        {            
            left_clicked = true;
            left_x_pos = x;
            left_y_pos = y;
        }
        else if(state == GLUT_UP){
            left_clicked = false;
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

    MeshGeometry_t* nacamesh = new MeshGeometry_t("./assets/mesh_ONERAM6_inv_ffd.su2");
    
    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    
    Transparent_t* air = new Transparent_t(0, airabsorber);
    Diffuse_t* difpurple = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 0.7, 0.85), 1);
    
    TransformMatrix_t* transform_light = new TransformMatrix_t();
    TransformMatrix_t* transform_camera = new TransformMatrix_t(); 
    TransformMatrix_t* transform_naca = new TransformMatrix_t();

    Mesh_t* naca = new Mesh_t(difpurple, transform_naca, nacamesh);
    themesh = naca;

    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements created in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    focus_point = Vec3f();
    naca->transformation_->translate(Vec3f(0.0, 0.0, 0.0));
    naca->transformation_->scale(1.0/100.0);
    naca->transformation_->rotateX(0.0);
    naca->transformation_->rotateZ(0.0);

    t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Elements transformed in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    Scene_t* scene = new Scene_t();
    thescene = scene;

    scene->add(naca);

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

    std::cout << "Max coord: " << naca->maxcoord()[0] << " " << naca->maxcoord()[1] << " " << naca->maxcoord()[2] << std::endl; // REMOVE
    std::cout << "Min coord: " << naca->mincoord()[0] << " " << naca->mincoord()[1] << " " << naca->mincoord()[2] << std::endl; // REMOVE

    // Camera stuff
    std::string filename = "./images/test.png";

    unsigned int res_x;
    unsigned int res_y;
    if (argc > 2){
        res_x = std::stoi(argv[1]);
        res_y = std::stoi(argv[2]);
    }
    else{
        res_x = 300;
        res_y = 200;
    }
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
    cam->transformation_->translate(Vec3f(0.0, -camera_dist, 0.0));
    cam->update();

    int gl_argc = 1;
    char* gl_argv[1];
    gl_argv[0] = argv[0];
    glutInit(&gl_argc, gl_argv); // was &argc, argv
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
    return 0;
}