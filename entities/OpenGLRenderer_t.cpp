#include "OpenGLRenderer_t.h"

#include <chrono>
#include <algorithm>
#include "Camera_t.h"
#include "Scene_t.h"
#include "ImgBufferOpenGL_t.h"
#include "TransformMatrix_t.h"

#ifdef _WIN32
#define GL_CLAMP_TO_EDGE 0x812F
#endif

OpenGLRenderer_t::OpenGLRenderer_t() :
    camera_(nullptr), scene_(nullptr), imgbuffer_(nullptr), width_(0), height_(0), 
    right_x_pos_(0), right_y_pos_(0), left_x_pos_(0), left_y_pos_(0), middle_x_pos_(0), 
    middle_y_pos_(0), right_clicked_(false), left_clicked_(false), middle_clicked_(false),
    n_iter_gl_(0), focus_point_(Vec3f()), camera_dist_(0), updated_(false) {}

OpenGLRenderer_t::OpenGLRenderer_t(Scene_t* scene, Camera_t* camera, ImgBufferOpenGL_t* imgbuffer) :
    camera_(camera), scene_(scene), imgbuffer_(imgbuffer), width_(0), height_(0), 
    right_x_pos_(0), right_y_pos_(0), left_x_pos_(0), left_y_pos_(0), middle_x_pos_(0), 
    middle_y_pos_(0), right_clicked_(false), left_clicked_(false), middle_clicked_(false),
    n_iter_gl_(0), focus_point_(Vec3f()), camera_dist_(0), updated_(false) {}

OpenGLRenderer_t::~OpenGLRenderer_t() {}

void OpenGLRenderer_t::raytrace(){
    if (updated_){
        TransformMatrix_t transform_norm = camera_->transformation_->transformDir();
        Vec3f newdir = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
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
    updated_ = false;
    glutPostRedisplay(); // REMOVE but makes it work, soooooooo...
}

void OpenGLRenderer_t::resetDisplay(void){
    camera_->reset();
    n_iter_gl_ = 0;
}

void OpenGLRenderer_t::mouseMovement(int x, int y){
    //Vec3f newdir = camera_->direction_;
    if (middle_clicked_){
        double differential_x = double(x - middle_x_pos_)/double(width_);
        double differential_y = double(y - middle_y_pos_)/double(height_);
        middle_x_pos_ = x;
        middle_y_pos_ = y;

        Vec3f horizontal = camera_->direction_.cross(camera_->up_);
        Vec3f vertical = horizontal.cross(camera_->direction_);

        focus_point_ += horizontal * -differential_x * camera_dist_ + vertical * differential_y * camera_dist_;
    }
    if (left_clicked_){
        //double differential_x = double(x - left_x_pos_)/double(width_);
        double differential_y = double(y - left_y_pos_)/double(height_);
        left_x_pos_ = x;
        left_y_pos_ = y;

        camera_dist_ *= std::pow(2.0, -differential_y);
    }
    if (right_clicked_){
        double differential_x = double(x - right_x_pos_)/double(width_);
        double differential_y = double(y - right_y_pos_)/double(height_);
        right_x_pos_ = x;
        right_y_pos_ = y;

        Vec3f horizontal = camera_->direction_.cross(camera_->up_);
        Vec3f vertical = horizontal.cross(camera_->direction_);

        camera_->transformation_->rotate(horizontal, differential_y/camera_->fov_[0]);
        camera_->transformation_->rotate(vertical, differential_x/camera_->fov_[1]);

        //TransformMatrix_t transform_norm = camera_->transformation_->transformDir();
        //newdir = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    }

    //Vec3f diff = focus_point_ - newdir * camera_dist_ - camera_->origin_;
    //camera_->transformation_->translate(diff);
    updated_ = true;
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
        std::cout << "Writing started. " << n_iter_gl_ << " iterations." << std::endl;
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
        glutDisplayFunc(this->raytrace);
        glutMouseFunc(this->mouseClick);
        glutMotionFunc(this->mouseMovement);
        glutKeyboardFunc(this->keyboard);
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
        position[0] = std::min(std::max(double(x)/double(width_), 0.0), 1.0);
        position[1] = std::min(std::max(double(y)/double(height_), 0.0), 1.0);
        camera_->autoFocus(scene_, position);
        updated_ = true;
        break;

    case 'q':
        glutDestroyWindow(0);
        exit(0);
        break;

    case 'p':
        std::cout << "Paused at " << n_iter_gl_ << " iterations." << std::endl;
        glutDisplayFunc(this->dummyDisp);
        glutMouseFunc(nullptr);
        glutMotionFunc(nullptr);
        glutKeyboardFunc(this->keyboardPaused);
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
    glutDisplayFunc(this->raytrace);
    glutMouseFunc(this->mouseClick);
    glutMotionFunc(this->mouseMovement);
    glutKeyboardFunc(this->keyboard);

    glGenTextures(1, &(this->imgbuffer_->tex_));
    glBindTexture(GL_TEXTURE_2D, this->imgbuffer_->tex_);
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