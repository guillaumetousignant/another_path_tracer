#include "RandomGenerator_t.h"
#include "Diffuse_t.h"
#include "NormalMaterial_t.h"
#include "Vec3f.h"
#include "Mesh_t.h"
#include "TransformMatrix_t.h"
#include "Scene_t.h"
#include "ImgBufferOpenGL_t.h"
#include "Cam_t.h"
#include "Camera_t.h"
#include <string>
#include <list>
#include "NonAbsorber_t.h"
#include "Transparent_t.h"
#include <string>
#include "DirectionalLight_t.h"
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

Camera_t* thecamera;
Scene_t* thescene;
Mesh_t* themesh;
double width, height;
int right_x_pos = 0;
int right_y_pos = 0;
int left_x_pos = 0;
int left_y_pos = 0;
int middle_x_pos = 0;
int middle_y_pos = 0;
bool right_clicked = false;
bool left_clicked = false;
bool middle_clicked = false;
int n_iter_gl = 0;
Vec3f focus_point = Vec3f();
double camera_dist = 5;

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
    Vec3f newdir = thecamera->direction_;
    if (middle_clicked){
        double differential_x = double(x - middle_x_pos)/double(width);
        double differential_y = double(y - middle_y_pos)/double(height);
        middle_x_pos = x;
        middle_y_pos = y;

        Vec3f horizontal = thecamera->direction_.cross(thecamera->up_);
        Vec3f vertical = horizontal.cross(thecamera->direction_);

        focus_point += horizontal * -differential_x * camera_dist + vertical * differential_y * camera_dist;
    }
    if (left_clicked){
        //double differential_x = double(x - left_x_pos)/double(width);
        double differential_y = double(y - left_y_pos)/double(height);
        left_x_pos = x;
        left_y_pos = y;

        camera_dist *= std::pow(2.0, -differential_y);
    }
    if (right_clicked){
        double differential_x = double(x - right_x_pos)/double(width);
        double differential_y = double(y - right_y_pos)/double(height);
        right_x_pos = x;
        right_y_pos = y;

        Vec3f horizontal = thecamera->direction_.cross(thecamera->up_);
        Vec3f vertical = horizontal.cross(thecamera->direction_);

        thecamera->transformation_->rotate(horizontal, differential_y/thecamera->fov_[0]);
        thecamera->transformation_->rotate(vertical, differential_x/thecamera->fov_[1]);

        TransformMatrix_t transform_norm = thecamera->transformation_->transformDir();
        newdir = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    }

    Vec3f diff = focus_point - newdir * camera_dist - thecamera->origin_;
    thecamera->transformation_->translate(diff);

    thecamera->update();
    thecamera->reset();
    n_iter_gl = 0;
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
            middle_clicked = true;
            middle_x_pos = x;
            middle_y_pos = y;
        }
        else if(state == GLUT_UP){
            middle_clicked = false;
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
    switch (key){
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
        thecamera->reset();
        thecamera->update(); // Should not be here!
        break;

    case 'q':
        glutDestroyWindow(0);
        exit(0);
        break;
    }
}
int main(int argc, char **argv){
    auto t_start = std::chrono::high_resolution_clock::now();

    std::string mesh_filename;
    if (argc > 1){
        mesh_filename = argv[1];
    }
    else{
        mesh_filename = "./assets/naca0012_coarse.su2";
    }

    MeshGeometry_t* nacamesh = new MeshGeometry_t(mesh_filename);
    
    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    
    Transparent_t* air = new Transparent_t(0, airabsorber);
    //Diffuse_t* difpurple = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 0.7, 0.85), 1);
    NormalMaterial_t* normalmat = new NormalMaterial_t();
    
    TransformMatrix_t* transform_light = new TransformMatrix_t();
    TransformMatrix_t* transform_camera = new TransformMatrix_t(); 
    TransformMatrix_t* transform_naca = new TransformMatrix_t();

    Mesh_t* naca = new Mesh_t(normalmat, transform_naca, nacamesh);
    themesh = naca;

    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements created in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    focus_point = Vec3f();
    /*naca->transformation_->translate(Vec3f(0.0, 0.0, 0.0));
    naca->transformation_->scale(1.0);
    naca->transformation_->rotateX(0.0);
    naca->transformation_->rotateZ(0.0);*/

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

    Vec3f mincoord = naca->mincoord();
    Vec3f maxcoord = naca->maxcoord();

    std::cout << "Min coord: " << mincoord[0] << " " << mincoord[1] << " " << mincoord[2] << std::endl;
    std::cout << "Max coord: " << maxcoord[0] << " " << maxcoord[1] << " " << maxcoord[2] << std::endl << std::endl;

    // Camera stuff
    std::string filename = "./images/test.png";

    unsigned int res_x;
    unsigned int res_y;
    if (argc > 3){
        res_x = std::stoi(argv[2]);
        res_y = std::stoi(argv[3]);
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
    unsigned int maxbounces = 1;

    DirectionalLight_t* dirlight = new DirectionalLight_t(Vec3f(5, 5, 4), transform_light);
    dirlight->transformation_->scale(0.95);
    dirlight->transformation_->rotateZ(-0.7854);
    dirlight->transformation_->rotateX(-1.1781);
    dirlight->update();
    
    SkyboxFlatSun_t* skybox = new SkyboxFlatSun_t(Vec3f(0.85, 0.85, 0.98), dirlight);
    ImgBufferOpenGL_t* imgbuffer = new ImgBufferOpenGL_t(res_x, res_y);

    std::list<Medium_t*> medium_list;
    medium_list.assign(2, air);

    RecCamAperture_t* cam = new RecCamAperture_t(transform_camera, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, medium_list, skybox, maxbounces, focal_length, aperture, 1.0);
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

    glGenTextures(1, &(imgbuffer->tex_));
    glBindTexture(GL_TEXTURE_2D, imgbuffer->tex_);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, res_x, res_y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );    

    glutMainLoop();
    return 0;
}