#include "ReadScene.h"

#include <list>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include "tinyxml2.h"
#include "Vec3f.h"
#include "Colours.h"
#include "NextFilename.h"

#include "Diffuse_t.h"
#include "DiffuseTex_t.h"
#include "Refractive_t.h"
#include "Reflective_t.h"
#include "ReflectiveRefractiveFuzz_t.h"
#include "ReflectiveRefractive_t.h"

#include "NonAbsorber_t.h"
#include "Absorber_t.h"
#include "Transparent_t.h"

#include "TransformMatrix_t.h"
#include "ImgBuffer_t.h"
#include "ImgBufferOpenGL_t.h"
#include "Scene_t.h"
#include "MeshGeometry_t.h"
#include "DirectionalLight_t.h"
#include "Texture_t.h"

#include "Sphere_t.h"
#include "SphereMotionblur_t.h"
#include "Triangle_t.h"
#include "TriangleMotionblur_t.h"
#include "Mesh_t.h"
#include "MeshMotionblur_t.h"

#include "Camera_t.h"
#include "Cam_t.h"
#include "CamAperture_t.h"
#include "CamMotionblur_t.h"
#include "CamMotionblurAperture_t.h"
#include "RecCam_t.h"
#include "RecCamAperture_t.h"
#include "RecCamMotionblur_t.h"
#include "RecCamMotionblurAperture_t.h"
#include "IsoCam_t.h"
#include "IsoCamAperture_t.h"
#include "IsoCamMotionblur_t.h"
#include "IsoCamMotionblurAperture_t.h"
#include "Cam3D_t.h"
#include "Cam3DAperture_t.h"
#include "Cam3DMotionblur_t.h"
#include "Cam3DMotionblurAperture_t.h"

#include "SkyboxFlat_t.h"
#include "SkyboxFlatSun_t.h"

#include "GL/glut.h"
#include "GL/gl.h"

#define PI 3.141592653589793238463
#ifdef _WIN32
#define GL_CLAMP_TO_EDGE 0x812F
#endif

namespace rendering{
    Camera_t* thecamera = nullptr;
    Scene_t* thescene = nullptr;
    ImgBufferOpenGL_t* imgbuffer_GL = nullptr;
    double width = 0;
    double height = 0;
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
    bool updated = false;

    void dummy_disp(){}

    void raytrace(){
        if (updated){
            TransformMatrix_t transform_norm = thecamera->transformation_->transformDir();
            Vec3f newdir = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
            thecamera->transformation_->translate(focus_point - newdir * camera_dist - thecamera->origin_);
        
            updated = false;
            thecamera->update();     
            thecamera->reset();
            n_iter_gl = 0;
        }
        n_iter_gl++;
        //auto t_start = std::chrono::high_resolution_clock::now();
        thecamera->raytrace(thescene);
        //auto t_end = std::chrono::high_resolution_clock::now();

        /*std::cout << "Iteration " << n_iter_gl << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;*/
        updated = false;
        glutPostRedisplay(); // REMOVE but makes it work, soooooooo...
    }

    void resetDisplay(void){
        thecamera->reset();
        n_iter_gl = 0;
    }

    void mouse_movement(int x, int y){
        //Vec3f newdir = thecamera->direction_;
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

            //TransformMatrix_t transform_norm = thecamera->transformation_->transformDir();
            //newdir = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
        }

        //Vec3f diff = focus_point - newdir * camera_dist - thecamera->origin_;
        //thecamera->transformation_->translate(diff);
        updated = true;
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

    void keyboard_paused(unsigned char key, int x, int y);
    void keyboard(unsigned char key, int x, int y){
        auto t_start_write = std::chrono::high_resolution_clock::now(); // why is this needed?
        auto t_end_write = t_start_write;
        double position[2];
        
        switch (key){
        case 's':
            std::cout << "Writing started at " << n_iter_gl << " iterations." << std::endl;
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
            updated = true;
            break;

        case 'q':
            glutDestroyWindow(0);
            exit(0);
            break;

        case 'p':
            std::cout << "Paused at " << n_iter_gl << " iterations." << std::endl;
            glutDisplayFunc(dummy_disp);
            glutMouseFunc(nullptr);
            glutMotionFunc(nullptr);
            glutKeyboardFunc(keyboard_paused);
            break;
        }
    }

    void keyboard_paused(unsigned char key, int x, int y){
        auto t_start_write = std::chrono::high_resolution_clock::now(); // why is this needed?
        auto t_end_write = t_start_write;

        switch (key){
            case 's':
            std::cout << "Writing started. " << n_iter_gl << " iterations." << std::endl;
            t_start_write = std::chrono::high_resolution_clock::now();
            thecamera->write();
            t_end_write = std::chrono::high_resolution_clock::now();

            std::cout << "Writing done in "
            << std::chrono::duration<double, std::milli>(t_end_write-t_start_write).count()/1000.0 
            << "s." << std::endl;
            break;

        case 'q':
            glutDestroyWindow(0);
            exit(0);
            break;

        case 'p':
            std::cout << "Unpaused" << std::endl;
            glutDisplayFunc(raytrace);
            glutMouseFunc(mouse_click);
            glutMotionFunc(mouse_movement);
            glutKeyboardFunc(keyboard);
            glutPostRedisplay();
            break;
        }
    }
}

void read_scene(const std::string &xml_filename){
    bool use_GL = true;
    std::string scene_name, new_filename, folder;

    #ifdef _WIN32
        folder = "images\\";
    #else
        folder = "images/";
    #endif

    tinyxml2::XMLDocument xml_scene;
	xml_scene.LoadFile(xml_filename.c_str());
    if (xml_scene.Error()){
        std::cout << "There was an error while opening or parsing file '" << xml_filename << "'. Exiting." << std::endl;
        return;
    }

    tinyxml2::XMLElement* xml_top = xml_scene.FirstChildElement();
    scene_name = xml_top->Attribute("name");
    new_filename = next_filename(folder + scene_name + ".png");

    std::cout << "Scene name: '" << scene_name << "'." << std::endl; // REMOVE
    std::cout << "Filename: '" << new_filename << "'." << std::endl; // REMOVE

    // Data
    unsigned int n_transform_matrices = 0;
    TransformMatrix_t * transform_matrices = nullptr;
    unsigned int n_textures = 0;
    Texture_t * textures = nullptr;
    unsigned int n_scatterers = 0;
    ScatteringFunction_t * scatterers = nullptr;
    unsigned int n_materials = 0;
    Material_t * materials = nullptr;
    unsigned int n_mesh_geometries = 0;
    MeshGeometry_t * mesh_geometries = nullptr;
    unsigned int n_objects = 0;
    Shape_t * objects = nullptr;
    unsigned int n_directional_lights = 0;
    DirectionalLight_t * directional_lights = nullptr;
    unsigned int n_skyboxes = 0;
    Skybox_t * skyboxes = nullptr;
    unsigned int n_imgbuffers = 0;
    ImgBuffer_t * imgbuffers = nullptr;
    unsigned int n_cameras = 0;
    Camera_t * cameras = nullptr;

    // Fields
    tinyxml2::XMLElement* xml_transform_matrices = xml_top->FirstChildElement("transform_matrices");
    tinyxml2::XMLElement* xml_textures = xml_top->FirstChildElement("textures");
    tinyxml2::XMLElement* xml_scatterers = xml_top->FirstChildElement("scatterers");
    tinyxml2::XMLElement* xml_materials = xml_top->FirstChildElement("materials");
    tinyxml2::XMLElement* xml_mesh_geometries = xml_top->FirstChildElement("mesh_geometries");
    tinyxml2::XMLElement* xml_objects = xml_top->FirstChildElement("objects");
    tinyxml2::XMLElement* xml_directional_lights = xml_top->FirstChildElement("directional_lights");
    tinyxml2::XMLElement* xml_skyboxes = xml_top->FirstChildElement("skyboxes");
    tinyxml2::XMLElement* xml_imgbuffers = xml_top->FirstChildElement("imgbuffers");
    tinyxml2::XMLElement* xml_cameras = xml_top->FirstChildElement("cameras");

    std::cout << "Noot 1" << std::endl; // REMOVE

    // Transformation matrices
    if (xml_transform_matrices != nullptr){
        tinyxml2::XMLElement* xml_transform_matrix;
        for (xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            ++n_transform_matrices;
        }
    }

    std::cout << "Noot 2" << std::endl; // REMOVE

    if (xml_scatterers != nullptr){
        tinyxml2::XMLElement* xml_scatterer;
        std::string string_transform_matrix;
        for (xml_scatterer = xml_scatterers->FirstChildElement("scatterer"); xml_scatterer; xml_scatterer = xml_scatterer->NextSiblingElement("scatterer")){
            string_transform_matrix = "";
            string_transform_matrix = xml_scatterer->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    if (xml_materials != nullptr){
        tinyxml2::XMLElement* xml_material;
        std::string string_transform_matrix;
        for (xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            string_transform_matrix = "";
            string_transform_matrix = xml_material->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    if (xml_objects != nullptr){
        tinyxml2::XMLElement* xml_object;
        std::string string_transform_matrix;
        for (xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            string_transform_matrix = "";
            string_transform_matrix = xml_object->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    if (xml_directional_lights != nullptr){
        tinyxml2::XMLElement* xml_directional_light;
        std::string string_transform_matrix;
        for (xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            string_transform_matrix = "";
            string_transform_matrix = xml_directional_light->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    if (xml_skyboxes != nullptr){
        tinyxml2::XMLElement* xml_skybox;
        std::string string_transform_matrix;
        for (xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
            string_transform_matrix = "";
            string_transform_matrix = xml_skybox->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    if (xml_cameras != nullptr){
        tinyxml2::XMLElement* xml_camera;
        std::string string_transform_matrix;
        for (xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            string_transform_matrix = "";
            string_transform_matrix = xml_camera->Attribute("transform_matrix");
            std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
            if (string_transform_matrix == "nan"){
                ++n_transform_matrices;
            }
        }
    }

    std::cout << "Transform matrix count: " << n_transform_matrices << std::endl; // REMOVE





    auto t_start = std::chrono::high_resolution_clock::now();
    Texture_t* zombietex = new Texture_t("./assets/Zombie beast_texture5.png");
    //Texture_t* pipertex = new Texture_t("./assets/piper_pa18_obj/piper_diffuse.png");

    MeshGeometry_t* cubemesh = new MeshGeometry_t("./assets/cube.obj");
    MeshGeometry_t* zombiemesh = new MeshGeometry_t("./assets/Zombie_Beast4_test2.obj");
    //MeshGeometry_t* pipermesh = new MeshGeometry_t("./assets/piper_pa18_obj/piper_pa18.obj");
    //MeshGeometry_t* nacamesh = new MeshGeometry_t("./assets/mesh_ONERAM6_inv_ffd.su2");
    
    NonAbsorber_t* airabsorber = new NonAbsorber_t();
    Absorber_t* glassabsorber = new Absorber_t(Vec3f(), Vec3f(0.6, 0.95, 0.8), 100, 2.0);

    Transparent_t* air = new Transparent_t(0, airabsorber);
    Diffuse_t* difpurple = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.98, 0.7, 0.85), 1);
    Diffuse_t* diflight = new Diffuse_t(Vec3f(2.0, 2.0, 2.0), Vec3f(1.0, 1.0, 1.0), 1);
    Diffuse_t* difgreen = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.8, 0.95, 0.6), 1);
    Diffuse_t* difblue = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.1, 0.4, 0.8), 1);
    Diffuse_t* difgrey = new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.75, 0.75, 0.75), 0.5);
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
    TransformMatrix_t* transform6 = new TransformMatrix_t();
    TransformMatrix_t* transform_zombie = new TransformMatrix_t();
    //TransformMatrix_t* transform_piper = new TransformMatrix_t();
    TransformMatrix_t* transform_cube = new TransformMatrix_t();
    TransformMatrix_t* transform_neutral = new TransformMatrix_t();
    //TransformMatrix_t* transform_naca = new TransformMatrix_t();

    Sphere_t* spherepurple = new Sphere_t(difpurple, transform1);
    Sphere_t* mirror = new Sphere_t(ref1, transform2);
    Sphere_t* light = new Sphere_t(diflight, transform3);
    Sphere_t* sphereglass = new Sphere_t(glass, transform4);
    Sphere_t* ground = new Sphere_t(difgreen, transform5);
    SphereMotionblur_t* spheregrey = new SphereMotionblur_t(difgrey, transform6);

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
    MeshMotionblur_t* zombie = new MeshMotionblur_t(zombiemat, transform_zombie, zombiemesh);
    //Mesh_t* piper = new Mesh_t(pipermat, transform_piper, pipermesh);
    //Mesh_t* naca = new Mesh_t(difpurple, transform_naca, nacamesh);

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
    zombie->transformation_->translate(Vec3f(0.0, 0.0, -0.53));
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
    /*naca->transformation_->translate(Vec3f(0.0, 2.0, 0));
    naca->transformation_->scale(1.0/100.0);
    naca->transformation_->rotateX(0.0);
    naca->transformation_->rotateZ(0.0);*/
    spheregrey->transformation_->translate(Vec3f(-0.5, 0.0, 0.0));

    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elements transformed in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    Scene_t* scene = new Scene_t();
    rendering::thescene = scene;
    //scene->add(spherepurple);
    //scene->add(mirror);
    //scene->add(light);
    //scene->add(sphereglass);
    //scene->add(ground);
    scene->add(zombie);
    //scene->add(spheregrey);
    scene->add(planegrey1);
    scene->add(planegrey2);
    //scene->add(piper);
    //scene->add(naca);

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

    // Motion
    spheregrey->transformation_->translate(Vec3f(1.0, 0.0, 0.0));
    zombie->transformation_->rotateZ(-PI/16);
    scene->update();

    t_start = std::chrono::high_resolution_clock::now();
    scene->build_acc();
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Acceleration structure built in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl << std::endl;

    //std::cout << "Max coord: " << naca->maxcoord()[0] << " " << naca->maxcoord()[1] << " " << naca->maxcoord()[2] << std::endl; // REMOVE
    //std::cout << "Min coord: " << naca->mincoord()[0] << " " << naca->mincoord()[1] << " " << naca->mincoord()[2] << std::endl; // REMOVE

    // Camera stuff
    std::string filename = "./images/test.png";

    unsigned int res_x;
    unsigned int res_y;
    //if (argc > 2){
    //    res_x = std::stoi(argv[1]);
    //    res_y = std::stoi(argv[2]);
    //}
    //else{
        res_x = 300;
        res_y = 200;
    //}

    bool no_GL = false;
    /*if (argc > 3){
        std::string switch_GL = argv[3];
        std::transform(switch_GL.begin(), switch_GL.end(), switch_GL.begin(), ::tolower);
        if (switch_GL == "nogl"){
            no_GL = true;
        }
    }*/

    rendering::width = res_x;
    rendering::height = res_y;
    double fov[2];
    fov[1] = 80.0 * PI /180.0; 
    fov[0] = fov[1] * res_y/res_x;
    double fov_iso[2];
    fov_iso[1] = 2;
    fov_iso[0] = fov_iso[1] * res_y/res_x;
    unsigned int subpix[2] = {1, 1};
    unsigned int maxbounces = 16;
    double focal_length = 2.0;
    double aperture = 0.02;
    double time[2] = {0, 1};
    double eye_dist = 0.065;

    DirectionalLight_t* dirlight = new DirectionalLight_t(Vec3f(5.0, 5.0, 4.0), transform_light);
    dirlight->transformation_->scale(0.95);
    dirlight->transformation_->rotateZ(-0.7854);
    dirlight->transformation_->rotateX(-1.1781);
    dirlight->update();
    
    SkyboxFlatSun_t* skybox = new SkyboxFlatSun_t(Vec3f(0.85, 0.85, 0.98), dirlight);

    ImgBuffer_t* imgbuffer = nullptr;
    if (no_GL){
        imgbuffer = new ImgBuffer_t(res_x, res_y);
    }
    else{
        rendering::imgbuffer_GL = new ImgBufferOpenGL_t(res_x, res_y);
        imgbuffer = rendering::imgbuffer_GL;
    }
    ImgBuffer_t* imgbuffer_L = new ImgBuffer_t(res_x, res_y);
    ImgBuffer_t* imgbuffer_R = new ImgBuffer_t(res_x, res_y);


    std::list<Medium_t*> medium_list;
    medium_list.assign(2, air);

    Cam3DMotionblurAperture_t* cam = new Cam3DMotionblurAperture_t(transform_camera, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer, imgbuffer_L, imgbuffer_R, eye_dist, medium_list, skybox, maxbounces, focal_length, aperture, time, 1.0);
    rendering::thecamera = cam;
    cam->transformation_->translate(Vec3f(0.0, -2, 0.0));
    cam->update();
    cam->update();

    rendering::camera_dist = (Vec3f(0.0, 0.0, 0.0) - cam->origin_).magnitude();

    if (!use_GL){
        cam->accumulateWrite(scene, 10000, 100);
    }
    else{
        int gl_argc = 1;
        char* gl_argv[1];
        gl_argv[0] = (char*)"another_path_tracer";
        glutInit(&gl_argc, gl_argv); // was &argc, argv
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(res_x, res_y);
        glutInitWindowPosition(10,10);
        glutCreateWindow(gl_argv[0]);
        glutDisplayFunc(rendering::raytrace);
        glutMouseFunc(rendering::mouse_click);
        glutMotionFunc(rendering::mouse_movement);
        glutKeyboardFunc(rendering::keyboard);

        glGenTextures(1, &(rendering::imgbuffer_GL->tex_));
        glBindTexture(GL_TEXTURE_2D, rendering::imgbuffer_GL->tex_);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, 3, res_x, res_y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );    

        glutMainLoop();
    }
}