
#ifndef IMGBUFFER_T_H
#define IMGBUFFER_T_H

class ImgBuffer_t{
    public:
        ImgBuffer_t();
        ~ImgBuffer_t();

        void reset();
        void update();
        void set();

        unsigned int size_x_;
        unsigned int size_y_;
        unsigned int updates_;
        double** img_;
};
#endif