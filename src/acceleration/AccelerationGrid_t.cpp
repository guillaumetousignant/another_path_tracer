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

AccelerationGrid_t::AccelerationGrid_t(Shape_t** items, size_t n_items, size_t min_res, size_t max_res) : 
        level_(0), min_res_(min_res), max_res_(max_res) {
    Vec3f min1, max1;

    n_obj_ = n_items;

    std::array<Vec3f, 2> coordinates{Vec3f(std::numeric_limits<double>::infinity()),
                                     Vec3f(-std::numeric_limits<double>::infinity())};
    for (size_t i = 0; i < n_obj_; ++i) {
        coordinates[0].min(items[i]->mincoord());
        coordinates[1].max(items[i]->maxcoord());
    }

    const Vec3f grid_size = coordinates[1] - coordinates[0];
    bounding_box_ = Box_t(coordinates);

    const Vec3f cell_res = (grid_size * std::pow(n_obj_/(grid_size[0]*grid_size[1]*grid_size[2]), 1.0/3.0)).floor()
                            .max(min_res_)
                            .min(max_res_) - 1.0;

    for (unsigned int i = 0; i < 3; ++i) {
        cell_res_[i] = static_cast<size_t>(cell_res[i]+1.0);
    }

    cell_size_ = grid_size/(cell_res+1.0);
    cells_ = new GridCell_t*[cell_res_[0] *  cell_res_[1] * cell_res_[2]];
    for (size_t i = 0; i < cell_res_[0] * cell_res_[1] * cell_res_[2]; i++) {
        cells_[i] = nullptr;
    }

    for (size_t i = 0; i < n_obj_; ++i) {
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
        
        for (size_t z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
            for (size_t y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
                for (size_t x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                    if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] == nullptr) {
                        cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] = new GridCell_t;
                    }
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->add(items[i]);
                }
            }
        }
    }
}

AccelerationGrid_t::AccelerationGrid_t(Shape_t** items, size_t n_items, std::array<Vec3f, 2> coordinates, unsigned int level, size_t min_res, size_t max_res) : 
        level_(level), min_res_(min_res), max_res_(max_res) {
    Vec3f min1, max1;
    
    n_obj_ = n_items;

    const Vec3f grid_size = coordinates[1] - coordinates[0];
    bounding_box_ = Box_t(coordinates);

    const Vec3f cell_res = (grid_size * std::pow(n_obj_/(grid_size[0]*grid_size[1]*grid_size[2]), 1.0/3.0)).floor()
                            .max(min_res_)
                            .min(max_res_) - 1.0;

    for (unsigned int i = 0; i < 3; ++i) {
        cell_res_[i] = static_cast<size_t>(cell_res[i]+1.0);
    }

    cell_size_ = grid_size/(cell_res+1.0);
    cells_ = new GridCell_t*[cell_res_[0] *  cell_res_[1] * cell_res_[2]];
    for (size_t i = 0; i < cell_res_[0] * cell_res_[1] * cell_res_[2]; i++) {
        cells_[i] = nullptr;
    }

    for (size_t i = 0; i < n_obj_; ++i) {
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
        
        for (size_t z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
            for (size_t y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
                for (size_t x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
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
    for (size_t i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); ++i) {
        delete cells_[i];
    }
    delete [] cells_;
}

Shape_t* AccelerationGrid_t::intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const {
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
    
    for (unsigned int i = 0; i < 3; ++i) {
        if (ray.direction_[i] < 0) {
            deltat[i] = -cell_size_[i] * invdir[i];
            tnext[i] = tbbox + (cellcoord[i] * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = -1;
            cellstep[i] = -1;
        }
        else {
            deltat[i] = cell_size_[i] * invdir[i];
            tnext[i] = tbbox + ((cellcoord[i] + 1.0) * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = cell_res_[i];
            cellstep[i] = 1; 
        }
    }

    int cellcoordint[3] = {static_cast<int>(cellcoord[0]), static_cast<int>(cellcoord[1]), static_cast<int>(cellcoord[2])};
    Shape_t* hit_obj = nullptr;
    while (true) {
        if (cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]] != nullptr) {
            hit_obj = cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]]->intersect(ray, t, uv);
        }

        const int k = (tnext[0] < tnext[1]) * 4 + (tnext[0] < tnext[2]) * 2 + (tnext[1] < tnext[2]);
        const unsigned int nextaxis = map[k];

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

    for (size_t z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
        for (size_t y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
            for (size_t x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
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

    for (size_t z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
        for (size_t y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
            for (size_t x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] != nullptr) {
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->remove(item);
                }
            }
        }
    }
}

void AccelerationGrid_t::move(Shape_t* item) {

}