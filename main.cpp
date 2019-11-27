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
    unsigned int K_xyz;
    unsigned int p_Np_xyz;

    std::string eline;
    std::string xyzline;
    std::string etoken;
    std::string extoken;
    std::string eytoken;
    std::string eztoken;
    std::string xyztoken;
    std::string xtoken;
    std::string ytoken;
    std::string ztoken;
    std::string delimiter = ";";
    std::string dimdelimiter = ",";
    std::string e_filename = "assets/h_out.csv";
    std::string xyz_filename = "assets/xyz_out.csv";
    double sphere_size = 0.01;

    if (argc > 0) {
        e_filename = argv[1];
    }
    if (argc > 1) {
        xyz_filename = argv[2];
    }
    if (argc > 2) {
        sphere_size = std::stod(argv[3]);
    }

    std::ifstream efile(e_filename);
    if (!efile.is_open()) {
        std::cerr << "Error: file '" << e_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }
    std::ifstream xyzfile(xyz_filename);
    if (!xyzfile.is_open()) {
        std::cerr << "Error: file '" << xyz_filename << "' could not be opened. Exiting." << std::endl;
        exit(-2);
    }

    std::string e_header_string;
    std::string xyz_header_string;
    std::getline(efile, e_header_string);
    std::getline(xyzfile, xyz_header_string);
    std::stringstream e_header(e_header_string);
    std::stringstream xyz_header(xyz_header_string);
    e_header >> K >> p_Np;
    xyz_header >> K_xyz >> p_Np_xyz;

    if (K != K_xyz || p_Np != p_Np_xyz){
        std::cout << "Error: K or p_Np not equal in data and position file." << std::endl;
        exit(-3);
    }

    std::vector<Vec3f> data(K * p_Np);
    std::unique_ptr<DataMaterial_t> material(new DataMaterial_t(data.data()));
    std::vector<Shape_t*> spheres(K * p_Np);

    for (unsigned int k = 0; k < K; ++k){
        std::getline(efile, eline);
        std::getline(xyzfile, xyzline);
        unsigned int epos = 0;
        unsigned int xyzpos = 0;
        for (unsigned int p = 0; p < p_Np; ++p){
            epos = eline.find(delimiter);
            xyzpos = xyzline.find(delimiter);
            etoken = eline.substr(0, epos);
            xyztoken = xyzline.substr(0, xyzpos);         
            eline.erase(0, epos + delimiter.length());
            xyzline.erase(0, xyzpos + delimiter.length());

            epos = etoken.find(dimdelimiter);
            xyzpos = xyztoken.find(dimdelimiter);
            extoken = etoken.substr(0, epos);
            xtoken = etoken.substr(0, xyzpos);
            etoken.erase(0, epos + dimdelimiter.length());
            xyztoken.erase(0, xyzpos + dimdelimiter.length());
            epos = etoken.find(dimdelimiter);
            xyzpos = xyztoken.find(dimdelimiter);
            eytoken = etoken.substr(0, epos);
            ytoken = etoken.substr(0, xyzpos);
            etoken.erase(0, epos + dimdelimiter.length());
            xyztoken.erase(0, xyzpos + dimdelimiter.length());
            epos = etoken.find(dimdelimiter);
            xyzpos = xyztoken.find(dimdelimiter);
            eztoken = etoken.substr(0, epos);
            ztoken = etoken.substr(0, xyzpos);
            etoken.erase(0, epos + dimdelimiter.length());
            xyztoken.erase(0, xyzpos + dimdelimiter.length());

            data[p + p_Np*k] = Vec3f(std::stod(extoken), std::stod(eytoken), std::stod(eztoken))*1.0+ 0.5;
            //data[p + p_Np*k] = Vec3f(std::stod(xtoken), std::stod(ytoken), std::stod(ztoken));
            spheres[p + p_Np*k] = new Sphere_t(material.get(), new TransformMatrix_t(), p + p_Np*k);
            spheres[p + p_Np*k]->transformation_->translate(Vec3f(std::stod(xtoken), std::stod(ytoken), std::stod(ztoken)));
            spheres[p + p_Np*k]->transformation_->scale(sphere_size);
        }
    }

    efile.close();
    xyzfile.close();

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

    delete camera->transformation_;
    for (unsigned int i = 0; i < spheres.size(); ++i){
        delete spheres[i]->transformation_;
        delete spheres[i];
    }
    
    return 0;
}