#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <list>
#include <math.h>
#include "RandomGenerator_t.h"
#include "Shape_t.h"
#include "Sphere_t.h"
#include "DataMaterial_t.h"
#include "TransformMatrix_t.h"
#include "Scene_t.h"
#include "SkyboxFlat_t.h"
#include "ImgBuffer_t.h"
#include "ImgBufferOpenGL_t.h"
#include "Cam_t.h"
#include "Transparent_t.h"
#include "NonAbsorber_t.h"
#include "OpenGLRenderer_t.h"
#include "NextFilename.h"

#define PI 3.1416

int main(int argc, char **argv){
    unsigned int K;
    unsigned int p_Np;
    if (argc < 3){
        std::cout << "Arguments: K p_Np" << std::endl;
        exit(-1);
    }
    K = std::stoi(argv[1]);
    p_Np = std::stoi(argv[2]);

    std::string ex_filename = "assets/ex_out.csv";
    std::string ey_filename = "assets/ey_out.csv";
    std::string ez_filename = "assets/ez_out.csv";
    std::string hx_filename = "assets/hx_out.csv";
    std::string hy_filename = "assets/hy_out.csv";
    std::string hz_filename = "assets/hz_out.csv";
    std::string x_filename = "assets/x_out.csv";
    std::string y_filename = "assets/y_out.csv";
    std::string z_filename = "assets/z_out.csv";

    std::ifstream exfile(ex_filename);
    if (!exfile.is_open()) {
        std::cerr << "Error: file '" << ex_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream eyfile(ey_filename);
    if (!eyfile.is_open()) {
        std::cerr << "Error: file '" << ey_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream ezfile(ez_filename);
    if (!ezfile.is_open()) {
        std::cerr << "Error: file '" << ez_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream hxfile(hx_filename);
    if (!hxfile.is_open()) {
        std::cerr << "Error: file '" << hx_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream hyfile(hy_filename);
    if (!hyfile.is_open()) {
        std::cerr << "Error: file '" << hy_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream hzfile(hz_filename);
    if (!hzfile.is_open()) {
        std::cerr << "Error: file '" << hz_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream xfile(x_filename);
    if (!xfile.is_open()) {
        std::cerr << "Error: file '" << x_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream yfile(y_filename);
    if (!yfile.is_open()) {
        std::cerr << "Error: file '" << y_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream zfile(z_filename);
    if (!zfile.is_open()) {
        std::cerr << "Error: file '" << z_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }

    std::vector<Vec3f> data(K * p_Np);
    std::unique_ptr<DataMaterial_t> material(new DataMaterial_t(data.data()));
    std::vector<Shape_t*> spheres(K * p_Np);
    double sphere_size = 0.01;

    std::string exline;
    std::string eyline;
    std::string ezline;
    std::string hxline;
    std::string hyline;
    std::string hzline;
    std::string xline;
    std::string yline;
    std::string zline;
    std::string extoken;
    std::string eytoken;
    std::string eztoken;
    std::string hxtoken;
    std::string hytoken;
    std::string hztoken;
    std::string xtoken;
    std::string ytoken;
    std::string ztoken;
    std::string delimiter = ";";

    for (unsigned int k = 0; k < K; ++k){
        std::getline(exfile, exline);
        std::getline(eyfile, eyline);
        std::getline(ezfile, ezline);
        std::getline(hxfile, hxline);
        std::getline(hyfile, hyline);
        std::getline(hzfile, hzline);
        std::getline(xfile, xline);
        std::getline(yfile, yline);
        std::getline(zfile, zline);
        unsigned int expos = 0;
        unsigned int eypos = 0;
        unsigned int ezpos = 0;
        unsigned int hxpos = 0;
        unsigned int hypos = 0;
        unsigned int hzpos = 0;
        unsigned int xpos = 0;
        unsigned int ypos = 0;
        unsigned int zpos = 0;
        for (unsigned int p = 0; p < p_Np; ++p){
            expos = exline.find(delimiter);
            eypos = eyline.find(delimiter);
            ezpos = ezline.find(delimiter);
            hxpos = hxline.find(delimiter);
            hypos = hyline.find(delimiter);
            hzpos = hzline.find(delimiter);
            xpos = xline.find(delimiter);
            ypos = yline.find(delimiter);
            zpos = zline.find(delimiter);
            extoken = exline.substr(0, expos);
            eytoken = eyline.substr(0, eypos);
            eztoken = ezline.substr(0, ezpos);
            hxtoken = hxline.substr(0, hxpos);
            hytoken = hyline.substr(0, hypos);
            hztoken = hzline.substr(0, hzpos);
            xtoken = xline.substr(0, xpos);
            ytoken = yline.substr(0, ypos);
            ztoken = zline.substr(0, zpos);
            exline.erase(0, expos + delimiter.length());
            eyline.erase(0, eypos + delimiter.length());
            ezline.erase(0, ezpos + delimiter.length());
            hxline.erase(0, hxpos + delimiter.length());
            hyline.erase(0, hypos + delimiter.length());
            hzline.erase(0, hzpos + delimiter.length());
            xline.erase(0, xpos + delimiter.length());
            yline.erase(0, ypos + delimiter.length());
            zline.erase(0, zpos + delimiter.length());

            data[p + p_Np*k] = Vec3f(std::stod(hxtoken), std::stod(hytoken), std::stod(hztoken))*10.0 + 0.5;
            //data[p + p_Np*k] = Vec3f(std::stod(extoken), std::stod(eytoken), std::stod(eztoken))*1.0+ 0.5;
            //data[p + p_Np*k] = Vec3f(std::stod(xtoken), std::stod(ytoken), std::stod(ztoken));
            spheres[p + p_Np*k] = new Sphere_t(material.get(), new TransformMatrix_t(), p + p_Np*k);
            spheres[p + p_Np*k]->transformation_->translate(Vec3f(std::stod(xtoken), std::stod(ytoken), std::stod(ztoken)));
            spheres[p + p_Np*k]->transformation_->scale(sphere_size);
        }
    }

    std::unique_ptr<Scene_t> scene(new Scene_t());
    scene->add(spheres.data(), spheres.size());
    scene->update();
    scene->build_acc();

    std::unique_ptr<Skybox_t> skybox(new SkyboxFlat_t(Vec3f(1.0)));

    unsigned int resx = 600;
    unsigned int resy = 400;
    if (argc > 4){
        resx = std::stoi(argv[3]);
        resy = std::stoi(argv[4]);
    }
    std::unique_ptr<ImgBufferOpenGL_t> imgbuffer(new ImgBufferOpenGL_t(resx, resy));

    std::unique_ptr<ScatteringFunction_t> scatterer(new NonAbsorber_t());
    std::unique_ptr<Medium_t> medium(new Transparent_t(10, scatterer.get()));
    std::list<Medium_t*> medium_list = {medium.get(), medium.get()};

    double fov[2] = {40.0 * PI / 180.0, 60.0 * PI / 180.0};
    unsigned int subpix[2] = {1, 1};
    std::string folder = "images/";
    std::string scene_name = "data";
    std::string filename = next_filename(folder + scene_name + ".png");
    std::unique_ptr<Camera_t> camera(new Cam_t(new TransformMatrix_t, filename, Vec3f(0.0, 0.0, 1.0), fov, subpix, imgbuffer.get(), medium_list, skybox.get(), 1, 1.0));

    camera->transformation_->translate(Vec3f(0.0, -2.0, 0.0));
    camera->update();

    std::unique_ptr<OpenGLRenderer_t> opengl_renderer(new OpenGLRenderer_t(scene.get(), camera.get(), imgbuffer.get()));
    opengl_renderer->initialise();
    opengl_renderer->render();

    exfile.close();
    eyfile.close();
    ezfile.close();
    hxfile.close();
    hyfile.close();
    hzfile.close();
    xfile.close();
    yfile.close();
    zfile.close();

    delete camera->transformation_;
    for (unsigned int i = 0; i < spheres.size(); ++i){
        delete spheres[i]->transformation_;
        delete spheres[i];
    }
    
    return 0;
}