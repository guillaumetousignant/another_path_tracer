#include "acceleration/AccelerationGrid_t.h"
#include "entities/Shape_t.h"
#include "acceleration/GridCell_t.h"
#include <limits>
#include <cmath>

using APTracer::Acceleration::AccelerationGrid_t;
using APTracer::Shapes::Box_t;
using APTracer::Acceleration::GridCell_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Ray_t;

AccelerationGrid_t::AccelerationGrid_t(Shape_t** items, size_t n_items, const Vec3f* coordinates/* = nullptr*/, unsigned int level /* = 0*/, unsigned int min_res /* = 1 */, unsigned int max_res /* = 128 */) : 
        level_(level), min_res_(min_res), max_res_(max_res) {
    Vec3f min1, max1;
    unsigned int x, y, z;
    Vec3f bb_coordinates[2];

    n_obj_ = n_items;

    if (coordinates == nullptr) {
        bb_coordinates[0] = Vec3f(std::numeric_limits<double>::infinity());
        bb_coordinates[1] = Vec3f(-std::numeric_limits<double>::infinity());
        
        for (size_t i = 0; i < n_obj_; i++) {
            bb_coordinates[0].min(items[i]->mincoord());
            bb_coordinates[1].max(items[i]->maxcoord());
        }
    }
    else{
        bb_coordinates[0] = coordinates[0];
        bb_coordinates[1] = coordinates[1];
    }

    const Vec3f grid_size = bb_coordinates[1] - bb_coordinates[0];
    bounding_box_ = Box_t(bb_coordinates);

    const Vec3f cell_res = (grid_size * std::pow(n_obj_/(grid_size[0]*grid_size[1]*grid_size[2]), 1.0/3.0)).floor()
                            .max(min_res_)
                            .min(max_res_) - 1.0;

    for (unsigned int i = 0; i < 3; i++) {
        cell_res_[i] = (unsigned int)(cell_res[i]+1.0);
    }

    cell_size_ = grid_size/(cell_res+1.0);
    cells_ = new GridCell_t*[cell_res_[0] *  cell_res_[1] * cell_res_[2]];
    for (unsigned int i = 0; i < cell_res_[0] * cell_res_[1] * cell_res_[2]; i++) {
        cells_[i] = nullptr;
    }

    for (unsigned int i = 0; i < n_obj_; i++) {
        min1 = Vec3f(std::numeric_limits<double>::infinity());
        max1 = Vec3f(-std::numeric_limits<double>::infinity());
    
        min1.min(items[i]->mincoord());
        max1.max(items[i]->maxcoord());
        min1 = ((min1 - bounding_box_.coordinates_[0])/cell_size_).floor();
        max1 = ((max1 - bounding_box_.coordinates_[0])/cell_size_).floor();

        min1.max(0.0);
        max1.max(0.0);
        min1.min(cell_res);
        max1.min(cell_res);
        
        for (z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++) {
            for (y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++) {
                for (x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++) {
                    if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] == nullptr) {
                        cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] = new GridCell_t;
                    }
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->add(items[i]);
                }
            }
        }
    }
}

AccelerationGrid_t::~AccelerationGrid_t() {
    if (cells_ != nullptr) {
        for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++) {
            if (cells_[i] != nullptr) {
                delete cells_[i];
            }
        }
        delete [] cells_;
    }
}

Shape_t* AccelerationGrid_t::intersect(const Ray_t &ray, double &t, double (&uv)[2]) const {
    double tbbox;
    int cellexit[3] = {0, 0, 0};
    int cellstep[3] = {0, 0, 0};
    const unsigned int map[8] = {2, 1, 2, 1, 2, 2, 0, 0};

    t = std::numeric_limits<double>::infinity();
    const Vec3f invdir = Vec3f(1.0)/ray.direction_;

    if (!bounding_box_.intersection(ray, tbbox)) {
        return nullptr;
    }

    Vec3f deltat = Vec3f();
    Vec3f tnext = Vec3f();

    const Vec3f raycellorigin = (ray.origin_ + ray.direction_ * tbbox) - bounding_box_.coordinates_[0];
    const Vec3f cell_res = Vec3f(cell_res_[0], cell_res_[1], cell_res_[2]);
    const Vec3f cellcoord = (raycellorigin / cell_size_).floor()
                            .max(0.0)
                            .min(cell_res-1.0);
    
    for (unsigned int i = 0; i < 3; i++) {
        if (ray.direction_[i] < 0) {
            deltat[i] = -cell_size_[i] * invdir[i];
            tnext[i] = tbbox + (cellcoord[i] * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = -1;
            cellstep[i] = -1;
        }
        else{
            deltat[i] = cell_size_[i] * invdir[i];
            tnext[i] = tbbox + ((cellcoord[i] + 1.0) * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = cell_res_[i];
            cellstep[i] = 1; 
        }
    }

    int cellcoordint[3] = {(int)cellcoord[0], (int)cellcoord[1], (int)cellcoord[2]};
    Shape_t* hit_obj = nullptr;
    while (true) {
        if (cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]] != nullptr) {
            hit_obj = cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]]->intersect(ray, t, uv);
        }

        const int k = (tnext[0] < tnext[1]) * 4 + (tnext[0] < tnext[2]) * 2 + (tnext[1] < tnext[2]);
        const int nextaxis = map[k];

        if (t < tnext[nextaxis]) {
            break;
        }

        cellcoordint[nextaxis] = cellcoordint[nextaxis] + cellstep[nextaxis];
        if (cellcoordint[nextaxis] == cellexit[nextaxis]) {
            break;
        }
        tnext[nextaxis] = tnext[nextaxis] + deltat[nextaxis];
    }
    return hit_obj;
}

void AccelerationGrid_t::add(Shape_t* item) {
    Vec3f min1 = Vec3f(std::numeric_limits<double>::infinity());
    Vec3f max1 = Vec3f(-std::numeric_limits<double>::infinity());

    min1.min(item->mincoord());
    max1.max(item->maxcoord());
    min1 = (min1 - bounding_box_.coordinates_[0]).floor() /cell_size_;
    max1 = (max1 - bounding_box_.coordinates_[0]).floor() /cell_size_;
    min1.max(0.0);
    max1.max(0.0);

    const Vec3f cell_res = Vec3f(cell_res_[0], cell_res_[1], cell_res_[2]) - 1.0;
    min1.min(cell_res);
    max1.min(cell_res);

    for (unsigned int z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++) {
        for (unsigned int y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++) {
            for (unsigned int x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++) {
                if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] == nullptr) {
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] = new GridCell_t;
                }
                cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->add(item);
            }
        }
    }
}

void AccelerationGrid_t::remove(const Shape_t* item) {
    Vec3f min1 = Vec3f(std::numeric_limits<double>::infinity());
    Vec3f max1 = Vec3f(-std::numeric_limits<double>::infinity());

    min1.min(item->mincoord());
    max1.max(item->maxcoord());
    min1 = (min1 - bounding_box_.coordinates_[0]).floor() /cell_size_;
    max1 = (max1 - bounding_box_.coordinates_[0]).floor() /cell_size_;
    min1.max(0.0);
    max1.max(0.0);

    const Vec3f cell_res = Vec3f(cell_res_[0], cell_res_[1], cell_res_[2]) - 1.0;
    min1.min(cell_res);
    max1.min(cell_res);

    for (unsigned int z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++) {
        for (unsigned int y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++) {
            for (unsigned int x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++) {
                if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] != nullptr) {
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->remove(item);
                }
            }
        }
    }
}

void AccelerationGrid_t::move(Shape_t* item) {

}