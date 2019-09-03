#include "ImgBufferOpenGL_t.h"
#include <algorithm>

ImgBufferOpenGL_t::ImgBufferOpenGL_t(unsigned int size_x, unsigned int size_y): ImgBuffer_t(size_x, size_y) {
    img_gl_ = new unsigned char[size_x_*size_y_*3];
    for (unsigned int i = 0; i < size_x_*size_y_*3; i++){
        img_gl_[i] = 0;
    }
    tex_ = 0;
}

ImgBufferOpenGL_t::~ImgBufferOpenGL_t(){
    if (img_gl_ != nullptr){
        delete [] img_gl_;
    }
}

void ImgBufferOpenGL_t::reset(){
    updates_ = 0;
    for (unsigned int j = 0; j < size_y_; j++){
        for (unsigned int i = 0; i < size_x_; i++){
            img_[j][i] = Vec3f();
        }
    }
}

void ImgBufferOpenGL_t::update(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    ++updates_;

    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            img_[j][i] += img[j][i];
        }
    }
}

void ImgBufferOpenGL_t::update() {
    ++updates_;
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity(); 

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); 

    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, tex_ );
    glTexSubImage2D
        (
        GL_TEXTURE_2D, 0,
        0, 0,
        size_x_, size_y_,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        &img_gl_[0]
        );

    glBegin( GL_QUADS );
    glTexCoord2i( 0, 0 );
    glVertex2i( -1, -1 );
    glTexCoord2i( 1, 0 );
    glVertex2i(  1, -1 );
    glTexCoord2i( 1, 1 );
    glVertex2i(  1,  1 );
    glTexCoord2i( 0, 1 );
    glVertex2i( -1,  1 );
    glEnd();

    glutSwapBuffers();
}

void ImgBufferOpenGL_t::update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y) {
    //img_[pos_y][pos_x] = img_[pos_y][pos_x] * (1.0 - 1.0/(double)updates_) + colour/(double)updates_;
    img_[pos_y][pos_x] += colour;
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3] = std::lround(std::max(std::min(1.0, img_[pos_y][pos_x][0]/updates_), 0.0) * 255.0);
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3+1] = std::lround(std::max(std::min(1.0, img_[pos_y][pos_x][1]/updates_), 0.0) * 255.0);
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3+2] = std::lround(std::max(std::min(1.0, img_[pos_y][pos_x][2]/updates_), 0.0) * 255.0);
}

void ImgBufferOpenGL_t::set(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_ = 1;
    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            img_[j][i] = img[j][i];
        }
    }
}

void ImgBufferOpenGL_t::set(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y){
    img_[pos_y][pos_x] = colour;
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3] = (unsigned char)(std::min(1.0, img_[pos_y][pos_x][0]/updates_) * 255.0 );
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3+1] = (unsigned char)(std::min(1.0, img_[pos_y][pos_x][1]/updates_) * 255.0 );
    img_gl_[((size_y_-1-pos_y)*size_x_ + pos_x)*3+2] = (unsigned char)(std::min(1.0, img_[pos_y][pos_x][2]/updates_) * 255.0 );
}