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
     * This class interacts with OpenGL. It binds an ImgBufferOpenGL_t to an OpenGL texture and sets up OpenGL to display it.
     * It also implements some input functions, like mouse movement and keyboard clicks. The camera's raytrace function is called 
     * each frame, and the camera is reset if it is moved. It uses a singleton pattern and all functions are duplicated to
     * work with the OpenGL C-style API.
     */
    class OpenGLRenderer_t{
        public:
            /**
             * @brief Construct a new OpenGLRenderer_t object with default state and no bound objects.
             * 
             * The newly created instance is assigned to the renderer_ singleton. If already assigned, 
             * the old object is deleted.
             * It is up to the user to set those values after creation.
             */
            OpenGLRenderer_t();

            /**
             * @brief Construct a new OpenGLRenderer_t object with bound objects and default state.
             * 
             * The newly created instance is assigned to the renderer_ singleton. If already assigned, 
             * the old object is deleted.
             * 
             * @param scene Scene that will be rendered by the camera and used for autofocus.
             * @param camera Camera that will render the scene, and be moved by user input.
             * @param imgbuffer Image buffer to be displayed on the screen and used by the renderer.
             */
            OpenGLRenderer_t(Scene_t* scene, Camera_t* camera, ImgBufferOpenGL_t* imgbuffer);

            /**
             * @brief Destroy the OpenGLRenderer_t object, freeing the ressources held by it and setting renderer_ as nullptr.
             */
            ~OpenGLRenderer_t();

            static OpenGLRenderer_t* renderer_; /**< @brief Single OpenGLRenderer_t instance to be used by OpenGL.*/
            Camera_t* camera_ = nullptr; /**< @brief Camera used to render the scene, and be moved by user input.*/
            Scene_t* scene_ = nullptr; /**< @brief Scene rendered by the camera and used for autofocus.*/
            ImgBufferOpenGL_t* imgbuffer_ = nullptr; /**< @brief Image buffer displayed on the screen and used by the renderer.*/
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