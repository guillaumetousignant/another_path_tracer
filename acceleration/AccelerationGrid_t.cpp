#include "AccelerationGrid_t.h"
#include "Shape_t.h"
#include "GridCell_t.h"
#include "Box_t.h"
#include <limits>
#include <math.h>

AccelerationGrid_t::AccelerationGrid_t(Shape_t** items, unsigned int n_items) 
    : n_obj_(n_items) {
    Vec3f grid_size;
    Vec3f min1, max1;
    unsigned int x, y, z;

    coordinates_[0] = Vec3f(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    coordinates_[1] = Vec3f(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    
    for (unsigned int i = 0; i < n_obj_; i++){
        coordinates_[0].min(items[i]->mincoord());
        coordinates_[1].max(items[i]->maxcoord());
    }

    grid_size = coordinates_[1] - coordinates_[0];
    bounding_box_ = new Box_t(coordinates_);

    cell_res_ = (grid_size * std::pow(n_obj_/(grid_size[0]*grid_size[1]*grid_size[2]), 3)).floor();

    for (unsigned int i = 0; i < 3; i++){
        if (cell_res_[i] < 1){
            cell_res_[i] = 1;
        }
        else if(cell_res_[i] > 128){
            cell_res_[i] = 128;
        }
    }

    cell_size_ = grid_size/cell_res_;
    cells_ = new AccelerationStructure_t*[(unsigned int)cell_res_[0] * (unsigned int)cell_res_[1] * (unsigned int)cell_res_[2]];
    for (unsigned int i = 0; i < (unsigned int)cell_res_[0] * (unsigned int)cell_res_[1] * (unsigned int)cell_res_[2]; i++){
        cells_[i] = nullptr;
    }

    for (unsigned int i = 0; i < n_obj_; i++){
        min1 = Vec3f(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        max1 = Vec3f(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    
        min1.min(items[i]->mincoord());
        max1.max(items[i]->maxcoord());
        min1 = (min1 - coordinates_[0]).floor() /cell_size_;
        max1 = (max1 - coordinates_[0]).floor() /cell_size_;

        for (unsigned int i = 0; i < 3; i++){
            if(min1[i] < 0){
                min1[i] = 0;
            }
            if(max1[i] < 0){
                max1[i] = 0;
            }
        }

        min1.min(cell_res_-1.0);
        max1.min(cell_res_-1.0);

        for (z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++){
            for (y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++){
                for (x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++){
                    if (cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]] == nullptr){
                        cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]] = new GridCell_t;
                    }
                    cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]]->add(items[i]);
                }
            }
        }
    }
}

AccelerationGrid_t::~AccelerationGrid_t(){
    if (bounding_box_ != nullptr){
        delete bounding_box_;
    }

    if (cells_ != nullptr){
        for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++){
            if (cells_[i] != nullptr){
                delete cells_[i];
            }
        }
        delete [] cells_;
    }
}

void AccelerationGrid_t::update(){
    for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++){
        cells_[i]->update();
    }
}

void AccelerationGrid_t::intersect(const Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const {
    Vec3f invdir;

    invdir = Vec3f(1.0, 1.0, 1.0)/ray.direction_;

}

void AccelerationGrid_t::add(Shape_t* item){
    Vec3f min1, max1;

    min1 = Vec3f(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    max1 = Vec3f(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());

    min1.min(item->mincoord());
    max1.max(item->maxcoord());
    min1 = (min1 - coordinates_[0]).floor() /cell_size_;
    max1 = (max1 - coordinates_[0]).floor() /cell_size_;

    for (unsigned int i = 0; i < 3; i++){
        if(min1[i] < 0){
            min1[i] = 0;
        }
        if(max1[i] < 0){
            max1[i] = 0;
        }
    }

    min1.min(cell_res_-1.0);
    max1.min(cell_res_-1.0);

    for (unsigned int z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++){
        for (unsigned int y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++){
            for (unsigned int x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++){
                if (cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]] == nullptr){
                    cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]] = new GridCell_t;
                }
                cells_[x + y*(unsigned int)cell_size_[0] + z*(unsigned int)cell_size_[0]*(unsigned int)cell_size_[1]]->add(item);
            }
        }
    }
}

void AccelerationGrid_t::remove(const Shape_t* item){

}

void AccelerationGrid_t::move(Shape_t* item){

}