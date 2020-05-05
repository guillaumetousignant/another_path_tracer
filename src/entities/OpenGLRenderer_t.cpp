#include "entities/OpenGLRenderer_t.h"

#ifdef _WIN32
    #include "GL/freeglut.h"
#else
    #include "GL/glut.h"
#endif
#include "GL/gl.h"

#include <chrono>
#include <algorithm>
#include "entities/Camera_t.h"
#include "entities/Scene_t.h"
#include "entities/ImgBufferOpenGL_t.h"
#include "entities/TransformMatrix_t.h"

#ifdef _WIN32
    #define GL_CLAMP_TO_EDGE 0x812F
#endif

using APTracer::Entities::OpenGLRenderer_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Camera_t;
using APTracer::Entities::ImgBufferOpenGL_t;
using APTracer::Entities::Vec3f;

OpenGLRenderer_t* OpenGLRenderer_t::renderer_ = nullptr;

OpenGLRenderer_t::OpenGLRenderer_t() :
    camera_(nullptr), scene_(nullptr), imgbuffer_(nullptr), 
    right_x_pos_(0), right_y_pos_(0), left_x_pos_(0), left_y_pos_(0), middle_x_pos_(0), 
    middle_y_pos_(0), right_clicked_(false), left_clicked_(false), middle_clicked_(false),
    n_iter_gl_(0), focus_point_(Vec3f()), camera_dist_(0), updated_(false), write_interval_(1),
    render_function_(openGL_accumulate) {
        if (renderer_ != nullptr) {
            delete renderer_;
        }
        renderer_ = this;
}

OpenGLRenderer_t::OpenGLRenderer_t(Scene_t* scene, Camera_t* camera, ImgBufferOpenGL_t* imgbuffer) :
    camera_(camera), scene_(scene), imgbuffer_(imgbuffer),
    right_x_pos_(0), right_y_pos_(0), left_x_pos_(0), left_y_pos_(0), middle_x_pos_(0), 
    middle_y_pos_(0), right_clicked_(false), left_clicked_(false), middle_clicked_(false),
    n_iter_gl_(0), focus_point_(Vec3f()), camera_dist_((focus_point_ - camera_->origin_).magnitude()), updated_(false), 
    write_interval_(1), render_function_(openGL_accumulate) {
        if (renderer_ != nullptr) {
            delete renderer_;
        }
        renderer_ = this;
        if (camera_dist_ < 0.1){
            camera_dist_ = 0.1;
        }
}

OpenGLRenderer_t::~OpenGLRenderer_t() {
    renderer_ = nullptr;
}

void OpenGLRenderer_t::accumulate(){
    if (updated_){
        const Vec3f newdir = camera_->transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
        camera_->transformation_->translate(focus_point_ - newdir * camera_dist_ - camera_->origin_);
    
        updated_ = false;
        camera_->update();     
        camera_->reset();
        n_iter_gl_ = 0;
    }
    n_iter_gl_++;
    //auto t_start = std::chrono::high_resolution_clock::now();
    camera_->raytrace(scene_);
    //auto t_end = std::chrono::high_resolution_clock::now();

    /*std::cout << "Iteration " << n_iter_gl_ << " done in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl;*/

    glutPostRedisplay(); // REMOVE but makes it work, soooooooo...
}

void OpenGLRenderer_t::accumulate_write(){
    if (updated_){
        const Vec3f newdir = camera_->transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
        camera_->transformation_->translate(focus_point_ - newdir * camera_dist_ - camera_->origin_);
    
        updated_ = false;
        camera_->update();     
        camera_->reset();
        n_iter_gl_ = 0;
    }
    n_iter_gl_++;
    //auto t_start = std::chrono::high_resolution_clock::now();
    camera_->raytrace(scene_);
    //auto t_end = std::chrono::high_resolution_clock::now();

    if (!(n_iter_gl_%write_interval_)){
        std::cout << "Writing started at " << n_iter_gl_ << " iterations." << std::endl;
        auto t_start = std::chrono::high_resolution_clock::now();
        camera_->write();
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Writing done in "
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;
    }

    /*std::cout << "Iteration " << n_iter_gl_ << " done in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl;*/

    glutPostRedisplay(); // REMOVE but makes it work, soooooooo...
}

void OpenGLRenderer_t::resetDisplay(void){
    camera_->reset();
    n_iter_gl_ = 0;
}

void OpenGLRenderer_t::mouseMovement(int x, int y){
    //Vec3f newdir = camera_->direction_;
    if (middle_clicked_){
        const double differential_x = double(x - middle_x_pos_)/double(glutGet(GLUT_WINDOW_WIDTH)); // maybe change those to glut width/height?
        const double differential_y = double(y - middle_y_pos_)/double(glutGet(GLUT_WINDOW_HEIGHT)); // maybe change those to glut width/height?
        middle_x_pos_ = x;
        middle_y_pos_ = y;

        const Vec3f horizontal = camera_->direction_.cross(camera_->up_);
        const Vec3f vertical = horizontal.cross(camera_->direction_);

        focus_point_ += horizontal * -differential_x * camera_dist_ + vertical * differential_y * camera_dist_;
        updated_ = true;
    }
    if (left_clicked_){
        //double differential_x = double(x - left_x_pos_)/double(glutGet(GLUT_WINDOW_WIDTH));
        const double differential_y = double(y - left_y_pos_)/double(glutGet(GLUT_WINDOW_HEIGHT)); // maybe change those to glut width/height?
        left_x_pos_ = x;
        left_y_pos_ = y;

        camera_dist_ *= std::pow(2.0, -differential_y);
        updated_ = true;
    }
    if (right_clicked_){
        const double differential_x = double(x - right_x_pos_)/double(glutGet(GLUT_WINDOW_WIDTH)); // maybe change those to glut width/height?
        const double differential_y = double(y - right_y_pos_)/double(glutGet(GLUT_WINDOW_HEIGHT)); // maybe change those to glut width/height?
        right_x_pos_ = x;
        right_y_pos_ = y;

        const Vec3f horizontal = camera_->direction_.cross(camera_->up_);
        const Vec3f vertical = horizontal.cross(camera_->direction_);

        camera_->transformation_->rotate(horizontal, differential_y/camera_->fov_[0]);
        camera_->transformation_->rotate(vertical, differential_x/camera_->fov_[1]);
        updated_ = true;

        //newdir = camera_->transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
    }

    //Vec3f diff = focus_point_ - newdir * camera_dist_ - camera_->origin_;
    //camera_->transformation_->translate(diff);
    //updated_ = true;
}

void OpenGLRenderer_t::mouseClick(int button, int state, int x, int y){
    switch (button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){            
                left_clicked_ = true;
                left_x_pos_ = x;
                left_y_pos_ = y;
            }
            else if(state == GLUT_UP){
                left_clicked_ = false;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN){            
                middle_clicked_ = true;
                middle_x_pos_ = x;
                middle_y_pos_ = y;
            }
            else if(state == GLUT_UP){
                middle_clicked_ = false;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN){            
                right_clicked_ = true;
                right_x_pos_ = x;
                right_y_pos_ = y;
            }
            else if(state == GLUT_UP){
                right_clicked_ = false;
            }
            break;
    }
}

void OpenGLRenderer_t::keyboardPaused(unsigned char key, int x, int y){
    auto t_start_write = std::chrono::high_resolution_clock::now(); // why is this needed?
    auto t_end_write = t_start_write;

    switch (key){
        case 's':
        std::cout << "Writing started at " << n_iter_gl_ << " iterations." << std::endl;
        t_start_write = std::chrono::high_resolution_clock::now();
        camera_->write();
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
        glutDisplayFunc(render_function_);
        glutMouseFunc(openGL_mouseClick);
        glutMotionFunc(openGL_mouseMovement);
        glutKeyboardFunc(openGL_keyboard);
        glutPostRedisplay();
        break;
    }
}

void OpenGLRenderer_t::keyboard(unsigned char key, int x, int y){
    auto t_start_write = std::chrono::high_resolution_clock::now(); // why is this needed?
    auto t_end_write = t_start_write;
    double position[2];
    
    switch (key){
    case 's':
        std::cout << "Writing started at " << n_iter_gl_ << " iterations." << std::endl;
        t_start_write = std::chrono::high_resolution_clock::now();
        camera_->write();
        t_end_write = std::chrono::high_resolution_clock::now();

        std::cout << "Writing done in "
        << std::chrono::duration<double, std::milli>(t_end_write-t_start_write).count()/1000.0 
        << "s." << std::endl;
        break;

    case 'f':
        position[0] = std::min(std::max(double(x)/double(glutGet(GLUT_WINDOW_WIDTH)), 0.0), 1.0);
        position[1] = std::min(std::max(double(y)/double(glutGet(GLUT_WINDOW_HEIGHT)), 0.0), 1.0);
        camera_->autoFocus(scene_, position);
        updated_ = true;
        break;

    case 'q':
        glutDestroyWindow(0);
        exit(0);
        break;

    case 'p':
        std::cout << "Paused at " << n_iter_gl_ << " iterations." << std::endl;
        glutDisplayFunc(openGL_dummyDisp);
        glutMouseFunc(nullptr);
        glutMotionFunc(nullptr);
        glutKeyboardFunc(openGL_keyboardPaused);
        break;
    }
}

void OpenGLRenderer_t::initialise(){
    int gl_argc = 1;
    char* gl_argv[1];
    gl_argv[0] = (char*)"another_path_tracer";
    glutInit(&gl_argc, gl_argv); // was &argc, argv
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(imgbuffer_->size_x_, imgbuffer_->size_y_);
    glutInitWindowPosition(10,10);
    glutCreateWindow(gl_argv[0]);
    glutDisplayFunc(render_function_);
    glutMouseFunc(openGL_mouseClick);
    glutMotionFunc(openGL_mouseMovement);
    glutKeyboardFunc(openGL_keyboard);

    glGenTextures(1, &(imgbuffer_->tex_));
    glBindTexture(GL_TEXTURE_2D, imgbuffer_->tex_);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, imgbuffer_->size_x_, imgbuffer_->size_y_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );    
}

void OpenGLRenderer_t::render(){
    glutMainLoop();
}

void OpenGLRenderer_t::render_write(unsigned int write_interval /* = 1 */){
    write_interval_ = write_interval;
    render_function_ = openGL_accumulate_write;
    glutDisplayFunc(render_function_);
    glutMainLoop();
}

void OpenGLRenderer_t::openGL_dummyDisp(){
    renderer_->dummyDisp();
}

void OpenGLRenderer_t::openGL_accumulate(){
    renderer_->accumulate();
}

void OpenGLRenderer_t::openGL_accumulate_write(){
    renderer_->accumulate_write();
}

void OpenGLRenderer_t::openGL_resetDisplay(void){
    renderer_->resetDisplay();
}

void OpenGLRenderer_t::openGL_mouseMovement(int x, int y){
    renderer_->mouseMovement(std::forward<int>(x), std::forward<int>(y));
}

void OpenGLRenderer_t::openGL_mouseClick(int button, int state, int x, int y){
    renderer_->mouseClick(std::forward<int>(button), std::forward<int>(state), std::forward<int>(x), std::forward<int>(y));
}

void OpenGLRenderer_t::openGL_keyboardPaused(unsigned char key, int x, int y){
    renderer_->keyboardPaused(std::forward<unsigned char>(key), std::forward<int>(x), std::forward<int>(y));
}

void OpenGLRenderer_t::openGL_keyboard(unsigned char key, int x, int y){
    renderer_->keyboard(std::forward<unsigned char>(key), std::forward<int>(x), std::forward<int>(y));
}