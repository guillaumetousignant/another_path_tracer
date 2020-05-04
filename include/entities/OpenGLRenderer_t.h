#ifndef OPENGLRENDERER_T_H
#define OPENGLRENDERER_T_H

#include <string>
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Camera_t;
    class Scene_t;
    class ImgBufferOpenGL_t;

    /**
     * @brief The OpenGL renderer class displays an image on the screen and handles user input.
     * 
     * 
     */
    class OpenGLRenderer_t{
        public:
            OpenGLRenderer_t();
            OpenGLRenderer_t(Scene_t* scene, Camera_t* camera, ImgBufferOpenGL_t* imgbuffer);
            ~OpenGLRenderer_t();

            static OpenGLRenderer_t* renderer_;
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
            unsigned int write_interval_;
            void (*render_function_)();

            void dummyDisp(){};
            void accumulate();
            void accumulate_write();
            void resetDisplay(void);
            void mouseMovement(int x, int y);
            void mouseClick(int button, int state, int x, int y);
            void keyboardPaused(unsigned char key, int x, int y);
            void keyboard(unsigned char key, int x, int y);
            void initialise();
            void render();
            void render_write(unsigned int write_interval = 1);
        
            static void openGL_dummyDisp();
            static void openGL_accumulate();
            static void openGL_accumulate_write();
            static void openGL_resetDisplay(void);
            static void openGL_mouseMovement(int x, int y);
            static void openGL_mouseClick(int button, int state, int x, int y);
            static void openGL_keyboardPaused(unsigned char key, int x, int y);
            static void openGL_keyboard(unsigned char key, int x, int y);
    };
}}
#endif