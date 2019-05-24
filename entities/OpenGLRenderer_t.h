#ifndef OPENGLRENDERER_T_H
#define OPENGLRENDERER_T_H

#include "GL/glut.h"
#include "GL/gl.h"
#include <string>
#include "Vec3f.h"

class Camera_t;
class Scene_t;
class ImgBufferOpenGL_t;

class OpenGLRenderer_t{
    public:
        OpenGLRenderer_t();
        OpenGLRenderer_t(Scene_t* scene, Camera_t* camera, ImgBufferOpenGL_t* imgbuffer);
        ~OpenGLRenderer_t();

        Camera_t* camera_ = nullptr;
        Scene_t* scene_ = nullptr;
        ImgBufferOpenGL_t* imgbuffer_ = nullptr;
        double width_;
        double height_;
        int right_x_pos_;
        int right_y_pos_;
        int left_x_pos_;
        int left_y_pos_;
        int middle_x_pos_;
        int middle_y_pos_;
        bool right_clicked_;
        bool left_clicked_;
        bool middle_clicked_;
        int n_iter_gl_;
        Vec3f focus_point_;
        double camera_dist_;
        bool updated_;

        void dummyDisp(){};
        void raytrace();
        void resetDisplay(void);
        void mouseMovement(int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void keyboardPaused(unsigned char key, int x, int y);
        void keyboard(unsigned char key, int x, int y);
        void initialise();
        void render();
};

#endif