#ifndef APTRACER_OPENGLRENDERER_T_H
#define APTRACER_OPENGLRENDERER_T_H

#include <string>
#include <memory>
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
     * work with the OpenGL C-style API, which uses function pointers for callbacks.
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
            ~OpenGLRenderer_t() = default;

            static std::unique_ptr<OpenGLRenderer_t> renderer_; /**< @brief Single OpenGLRenderer_t instance to be used by OpenGL.*/
            Camera_t* camera_ = nullptr; /**< @brief Camera used to render the scene, and be moved by user input.*/
            Scene_t* scene_ = nullptr; /**< @brief Scene rendered by the camera and used for autofocus.*/
            ImgBufferOpenGL_t* imgbuffer_ = nullptr; /**< @brief Image buffer displayed on the screen and used by the renderer.*/
            int right_x_pos_; /**< @brief x coordinate on the screen where the right mouse button was last clicked.*/
            int right_y_pos_; /**< @brief y coordinate on the screen where the right mouse button was last clicked.*/
            int left_x_pos_; /**< @brief x coordinate on the screen where the left mouse button was last clicked.*/
            int left_y_pos_; /**< @brief y coordinate on the screen where the left mouse button was last clicked.*/
            int middle_x_pos_; /**< @brief x coordinate on the screen where the middle mouse button was last clicked.*/
            int middle_y_pos_; /**< @brief y coordinate on the screen where the middle mouse button was last clicked.*/
            bool right_clicked_; /**< @brief True if the right mouse button is currently pressed.*/
            bool left_clicked_; /**< @brief True if the left mouse button is currently pressed.*/
            bool middle_clicked_; /**< @brief True if the middle mouse button is currently pressed.*/
            Vec3f focus_point_; /**< @brief Point around which the camera rotates.*/
            double camera_dist_; /**< @brief Distance from the focus point to the camera.*/
            bool updated_; /**< @brief True if the camera moved or has been changed and scene must be redrawn.*/
            unsigned int write_interval_; /**< @brief Writes to disk every write_interval_ samples.*/
            void (*render_function_)(); /**< @brief Function pointer to the function used at each frame for rendering the scene.*/

            /**
             * @brief Function used every frame when the rendering is on pause. Does nothing.
             */
            auto dummyDisp() -> void {};

            /**
             * @brief Function used every frame to accumulate one sample per pixel for the whole image.
             * 
             * Uses the renderer's camera, scene and image buffer. 
             */
            auto accumulate() -> void;

            /**
             * @brief Function used every frame to accumulate one sample per pixel for the whole image, and save the image every write_interval_ samples.
             * 
             * Uses the renderer's camera, scene and image buffer. 
             */
            auto accumulate_write() -> void;

            /**
             * @brief Resets the camera and its image buffer, and starts accumulating samples from scratch.
             */
            auto resetDisplay(void) -> void;

            /**
             * @brief Function called by OpenGL on mouse movement. Moves the camera.
             * 
             * Makes the camera move according to mouse input if a button is clicked. Left button is zoom, middle button is pan, and right button is rotate.
             * 
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            auto mouseMovement(int x, int y) -> void;

            /**
             * @brief Function called by OpenGL on mouse clicks. Saves this state for camera move.
             * 
             * @param button Which button of the mouse was pressed or depressed.
             * @param state Is the button pressed or depressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            auto mouseClick(int button, int state, int x, int y) -> void;

            /**
             * @brief Function called by OpenGL on keyboard presses. Used when the rendering is paused.
             * 
             * Responsible for resuming the rendering, writing to disk and quitting.
             * 
             * @param key Which key was pressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            auto keyboardPaused(unsigned char key, int x, int y) -> void;

            /**
             * @brief Function called by OpenGL on keyboard presses. Used when the rendering is running.
             * 
             * Responsible for pausing the rendering, writing to disk, quitting and focusing.
             * 
             * @param key Which key was pressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            auto keyboard(unsigned char key, int x, int y) -> void;

            /**
             * @brief Initialises the OpenGL environment and binds the render output to it.
             */
            auto initialise() -> void;

            /**
             * @brief Launches the rendering, calling glutMainLoop.
             */
            auto render() -> void;

            /**
             * @brief Launches the rendering, calling glutMainLoop, and save the image every sample.
             */
            auto render_write() -> void;

            /**
             * @brief Launches the rendering, calling glutMainLoop, and save the image every write_interval_ samples.
             * 
             * @param write_interval Number of samples between image saves.
             */
            auto render_write(unsigned int write_interval) -> void;
        
            /**
             * @brief Wrapper function used every frame when the rendering is on pause. Does nothing.
             * 
             * This calls the dummyDisp function on the singleton OpenGLRenderer_t object.
             */
            static auto openGL_dummyDisp() -> void;

            /**
             * @brief Wrapper function used every frame to accumulate one sample per pixel for the whole image.
             * 
             * Uses the renderer's camera, scene and image buffer. 
             * This calls the accumulate function on the singleton OpenGLRenderer_t object.
             */
            static auto openGL_accumulate() -> void;

            /**
             * @brief Wrapper function used every frame to accumulate one sample per pixel for the whole image, and save the image every write_interval_ samples.
             * 
             * Uses the renderer's camera, scene and image buffer. 
             * This calls the accumulate_write function on the singleton OpenGLRenderer_t object.
             */
            static auto openGL_accumulate_write() -> void;

            /**
             * @brief Wrapper function that resets the camera and its image buffer, and starts accumulating samples from scratch.
             * 
             * This calls the accumulate_write function on the singleton OpenGLRenderer_t object.
             */
            static auto openGL_resetDisplay(void) -> void;

            /**
             * @brief Wrapper function called by OpenGL on mouse movement. Moves the camera.
             * 
             * Makes the camera move according to mouse input if a button is clicked. Left button is zoom, middle button is pan, and right button is rotate.
             * This calls the mouseMovement function on the singleton OpenGLRenderer_t object.
             * 
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            static auto openGL_mouseMovement(int x, int y) -> void;

            /**
             * @brief Wrapper function called by OpenGL on mouse clicks. Saves this state for camera move.
             * 
             * This calls the mouseClick function on the singleton OpenGLRenderer_t object.
             * 
             * @param button Which button of the mouse was pressed or depressed.
             * @param state Is the button pressed or depressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            static auto openGL_mouseClick(int button, int state, int x, int y) -> void;

            /**
             * @brief Wrapper function called by OpenGL on keyboard presses. Used when the rendering is paused.
             * 
             * Responsible for resuming the rendering, writing to disk and quitting.
             * This calls the keyboardPaused function on the singleton OpenGLRenderer_t object.
             * 
             * @param key Which key was pressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            static auto openGL_keyboardPaused(unsigned char key, int x, int y) -> void;

            /**
             * @brief Wrapper function called by OpenGL on keyboard presses. Used when the rendering is running.
             * 
             * Responsible for pausing the rendering, writing to disk, quitting and focusing.
             * This calls the keyboardPaused function on the singleton OpenGLRenderer_t object.
             * 
             * @param key Which key was pressed.
             * @param x Horizontal position of the cursor in pixels along the window from the left side.
             * @param y Vertical position of the cursor in pixels along the window from the bottom.
             */
            static auto openGL_keyboard(unsigned char key, int x, int y) -> void;
    };
}}
#endif