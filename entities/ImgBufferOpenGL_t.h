#ifndef IMGBUFFEROPENGL_T_H
#define IMGBUFFEROPENGL_T_H

#include "ImgBuffer_t.h"
#include "Vec3f.h"
#include <string>

#include "GL/glut.h"
#include "GL/gl.h"

class ImgBufferOpenGL_t : public ImgBuffer_t{
    public:
        ImgBufferOpenGL_t(unsigned int size_x, unsigned int size_y);
        virtual ~ImgBufferOpenGL_t();

        virtual void reset();
        virtual void update(const Vec3f** img, unsigned int size_x, unsigned int size_y);
        virtual void update();
        virtual void update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y);
        virtual void set(const Vec3f** img, unsigned int size_x, unsigned int size_y);
        virtual void write(std::string &filename) const;

        unsigned char* img_gl_;
        GLuint tex_;
};
#endif