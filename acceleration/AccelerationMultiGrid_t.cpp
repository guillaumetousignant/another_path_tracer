#include "AccelerationMultiGrid_t.h"
#include "Shape_t.h"
#include "GridCell_t.h"
#include <limits>
#include <cmath>

AccelerationMultiGrid_t::AccelerationMultiGrid_t(Shape_t** items, unsigned int n_items, Vec3f* coordinates/* = nullptr*/, unsigned int level /* = 0*/, unsigned int min_res /* = 1 */, unsigned int max_res /* = 128 */, unsigned int max_cell_content /* = 32 */, unsigned int max_grid_level /* = 1 */) : 
        level_(level), min_res_(min_res), max_res_(max_res), max_cell_content_(max_cell_content), max_grid_level_(max_grid_level) {
    Vec3f grid_size;
    Vec3f min1, max1;
    Vec3f cell_res;
    unsigned int x, y, z;
    GridCell_t** temp_cells;
    Vec3f cell_extent[2];

    Shape_t** temp_elements = nullptr;
    unsigned int element_index;

    n_obj_ = n_items;

    if (coordinates == nullptr){
        coordinates_[0] = Vec3f(std::numeric_limits<double>::infinity());
        coordinates_[1] = Vec3f(-std::numeric_limits<double>::infinity());
        
        for (unsigned int i = 0; i < n_obj_; i++){
            coordinates_[0].min(items[i]->mincoord());
            coordinates_[1].max(items[i]->maxcoord());
        }
    }
    else{
        coordinates_[0] = coordinates[0];
        coordinates_[1] = coordinates[1];
    }

    grid_size = coordinates_[1] - coordinates_[0];
    bounding_box_ = Box_t(coordinates_);

    cell_res = (grid_size * std::pow(n_obj_/(grid_size[0]*grid_size[1]*grid_size[2]), 1.0/3.0)).floor();

    cell_res.max(min_res_);
    cell_res.min(max_res_);
    for (unsigned int i = 0; i < 3; i++){
        cell_res_[i] = (unsigned int)cell_res[i];
    }

    cell_size_ = grid_size/cell_res;
    cells_ = new AccelerationStructure_t*[cell_res_[0] *  cell_res_[1] * cell_res_[2]];
    temp_cells = new GridCell_t*[cell_res_[0] *  cell_res_[1] * cell_res_[2]];
    for (unsigned int i = 0; i < cell_res_[0] * cell_res_[1] * cell_res_[2]; i++){
        cells_[i] = nullptr;
        temp_cells[i] = nullptr;
    }

    for (unsigned int i = 0; i < n_obj_; i++){
        min1 = Vec3f(std::numeric_limits<double>::infinity());
        max1 = Vec3f(-std::numeric_limits<double>::infinity());
    
        min1.min(items[i]->mincoord());
        max1.max(items[i]->maxcoord());
        min1 = ((min1 - coordinates_[0])/cell_size_).floor();
        max1 = ((max1 - coordinates_[0])/cell_size_).floor();

        min1.max(0.0);
        max1.max(0.0);
        min1.min(cell_res-1.0);
        max1.min(cell_res-1.0);
        
        for (z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++){
            for (y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++){
                for (x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++){
                    if (temp_cells[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] == nullptr){
                        temp_cells[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] = new GridCell_t;
                    }
                    temp_cells[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->add(items[i]);
                }
            }
        }
    }

    for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++){
        if (temp_cells[i] != nullptr){
            if ((temp_cells[i]->n_obj_ > max_cell_content_) && (level_ < max_grid_level_)){
                temp_elements = new Shape_t*[temp_cells[i]->n_obj_];
                element_index = 0;
                for (auto it = temp_cells[i]->items_.begin(); it != temp_cells[i]->items_.end(); ++it){
                    temp_elements[element_index] = *it;
                    element_index++;
                }

                z = i/(cell_res_[0]*cell_res_[1]);
                y = (i - z * cell_res_[0]*cell_res_[1])/cell_res_[0];
                x = (i - y * cell_res_[0] - z * cell_res_[0]*cell_res_[1]);

                cell_extent[0] = coordinates_[0] + grid_size*Vec3f(x, y, z)/cell_res;
                cell_extent[1] = cell_extent[0] + cell_size_;

                cells_[i] = new AccelerationMultiGrid_t(temp_elements, temp_cells[i]->n_obj_, &cell_extent[0], level_+1, min_res_, max_res_, max_cell_content_, max_grid_level_);

                delete [] temp_elements;
                temp_elements = nullptr;
                delete temp_cells[i];
                temp_cells[i] = nullptr;

            }
            else{
                cells_[i] = temp_cells[i];
            }
        }
    }

    delete [] temp_cells;
}

AccelerationMultiGrid_t::~AccelerationMultiGrid_t(){
    if (cells_ != nullptr){
        for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++){
            if (cells_[i] != nullptr){
                delete cells_[i];
            }
        }
        delete [] cells_;
    }
}

void AccelerationMultiGrid_t::update(){
    for (unsigned int i = 0; i < (cell_res_[0]*cell_res_[1]*cell_res_[2]); i++){
        cells_[i]->update();
    }
}

void AccelerationMultiGrid_t::intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    Vec3f invdir;
    double tbbox;
    bool intersected;
    int cellexit[3] = {0, 0, 0};
    int cellstep[3] = {0, 0, 0};
    Vec3f deltat, tnext;
    Vec3f raycellorigin;
    Vec3f cellcoord;
    Vec3f cell_res;
    int k;
    int map[8] = {2, 1, 2, 1, 2, 2, 0, 0};
    int nextaxis;

    hit_obj = nullptr;
    t = std::numeric_limits<double>::infinity();
    invdir = Vec3f(1.0)/ray.direction_;

    bounding_box_.intersection(ray, intersected, tbbox);
    if (!intersected){
        return;
    }

    deltat = Vec3f();
    tnext = Vec3f();

    raycellorigin = (ray.origin_ + ray.direction_ * tbbox) - bounding_box_.coordinates_[0];
    cellcoord = (raycellorigin / cell_size_).floor();
    for (unsigned int i = 0; i < 3; i++){
        if (cellcoord[i] < 0.0){
            cellcoord[i] = 0.0;
        }
    }    
    cell_res = Vec3f(cell_res_[0], cell_res_[1], cell_res_[2]);
    cellcoord = cellcoord.min(cell_res-1.0);
    
    for (unsigned int i = 0; i < 3; i++){
        if (ray.direction_[i] < 0){
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

    while (true){
        if (cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]] != nullptr){
            cells_[cellcoordint[0] + cellcoordint[1]*cell_res_[0] + cellcoordint[2]*cell_res_[0]*cell_res_[1]]->intersect(ray, hit_obj, t, uv);
        }

        k = (tnext[0] < tnext[1]) * 4 + (tnext[0] < tnext[2]) * 2 + (tnext[1] < tnext[2]);
        nextaxis = map[k];

        if (t < tnext[nextaxis]){
            break;
        }

        cellcoordint[nextaxis] = cellcoordint[nextaxis] + cellstep[nextaxis];
        if (cellcoordint[nextaxis] == cellexit[nextaxis]){
            break;
        }
        tnext[nextaxis] = tnext[nextaxis] + deltat[nextaxis];
    }
}

void AccelerationMultiGrid_t::add(Shape_t* item){
    Vec3f min1, max1;
    Vec3f cell_res;

    min1 = Vec3f(std::numeric_limits<double>::infinity());
    max1 = Vec3f(-std::numeric_limits<double>::infinity());

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

    cell_res = Vec3f(cell_res_[0], cell_res_[1], cell_res_[2]);
    min1.min(cell_res-1.0);
    max1.min(cell_res-1.0);

    for (unsigned int z = (unsigned int)min1[2]; z <= (unsigned int)max1[2]; z++){
        for (unsigned int y = (unsigned int)min1[1]; y <= (unsigned int)max1[1]; y++){
            for (unsigned int x = (unsigned int)min1[0]; x <= (unsigned int)max1[0]; x++){
                if (cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] == nullptr){
                    cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]] = new GridCell_t;
                }
                cells_[x + y*cell_res_[0] + z*cell_res_[0]*cell_res_[1]]->add(item);
            }
        }
    }
}

void AccelerationMultiGrid_t::remove(const Shape_t* item){

}

void AccelerationMultiGrid_t::move(Shape_t* item){

}