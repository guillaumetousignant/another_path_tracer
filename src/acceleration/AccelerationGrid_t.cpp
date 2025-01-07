#include "acceleration/AccelerationGrid_t.hpp"
#include <cmath>
#include <limits>

using APTracer::Acceleration::AccelerationGrid_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Vec3f;
using APTracer::Shapes::Box_t;

AccelerationGrid_t::AccelerationGrid_t(const std::vector<Shape_t*>& items, size_t min_res, size_t max_res) : cell_res_(), level_(0), min_res_(min_res), max_res_(max_res), size_(items.size()) {

    std::array<Vec3f, 2> coordinates{Vec3f(std::numeric_limits<double>::max()), Vec3f(std::numeric_limits<double>::lowest())};
    for (const auto& item: items) {
        coordinates[0].min(item->mincoord());
        coordinates[1].max(item->maxcoord());
    }

    const Vec3f grid_size = coordinates[1] - coordinates[0];
    bounding_box_         = Box_t(coordinates);

    const Vec3f cell_res = (grid_size * std::pow(static_cast<double>(items.size()) / (grid_size[0] * grid_size[1] * grid_size[2]), 1.0 / 3.0))
                               .floor()
                               .max(static_cast<double>(min_res_))
                               .min(static_cast<double>(max_res_))
                         - 1.0;

    cell_res_ = {static_cast<size_t>(cell_res[0] + 1.0), static_cast<size_t>(cell_res[1] + 1.0), static_cast<size_t>(cell_res[2] + 1.0)};

    cell_size_ = grid_size / (cell_res + 1.0);
    cells_     = std::vector<std::unique_ptr<GridCell_t>>(cell_res_[0] * cell_res_[1] * cell_res_[2]);

    for (const auto& item: items) {
        Vec3f min1 = Vec3f(std::numeric_limits<double>::max());
        Vec3f max1 = Vec3f(std::numeric_limits<double>::lowest());

        min1.min(item->mincoord());
        max1.max(item->maxcoord());
        min1 = ((min1 - bounding_box_.coordinates_[0]) / cell_size_).floor();
        max1 = ((max1 - bounding_box_.coordinates_[0]) / cell_size_).floor();

        min1.max(0.0);
        max1.max(0.0);
        min1.min(cell_res);
        max1.min(cell_res);

        for (auto z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
            for (auto y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
                for (auto x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                    if (!cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]) {
                        cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]] = std::make_unique<GridCell_t>();
                    }
                    cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]->add(item);
                }
            }
        }
    }
}

AccelerationGrid_t::AccelerationGrid_t(const std::vector<Shape_t*>& items, std::array<Vec3f, 2> coordinates, unsigned int level, size_t min_res, size_t max_res) :
        cell_res_(), level_(level), min_res_(min_res), max_res_(max_res), size_(items.size()) {

    const Vec3f grid_size = coordinates[1] - coordinates[0];
    bounding_box_         = Box_t(coordinates);

    const Vec3f cell_res = (grid_size * std::pow(static_cast<double>(items.size()) / (grid_size[0] * grid_size[1] * grid_size[2]), 1.0 / 3.0))
                               .floor()
                               .max(static_cast<double>(min_res_))
                               .min(static_cast<double>(max_res_))
                         - 1.0;

    cell_res_ = {static_cast<size_t>(cell_res[0] + 1.0), static_cast<size_t>(cell_res[1] + 1.0), static_cast<size_t>(cell_res[2] + 1.0)};

    cell_size_ = grid_size / (cell_res + 1.0);
    cells_     = std::vector<std::unique_ptr<GridCell_t>>(cell_res_[0] * cell_res_[1] * cell_res_[2]);

    for (const auto& item: items) {
        Vec3f min1 = Vec3f(std::numeric_limits<double>::max());
        Vec3f max1 = Vec3f(std::numeric_limits<double>::lowest());

        min1.min(item->mincoord());
        max1.max(item->maxcoord());
        min1 = ((min1 - bounding_box_.coordinates_[0]) / cell_size_).floor();
        max1 = ((max1 - bounding_box_.coordinates_[0]) / cell_size_).floor();

        min1.max(0.0);
        max1.max(0.0);
        min1.min(cell_res);
        max1.min(cell_res);

        for (auto z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
            for (auto y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
                for (auto x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                    if (!cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]) {
                        cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]] = std::make_unique<GridCell_t>();
                    }
                    cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]->add(item);
                }
            }
        }
    }
}

AccelerationGrid_t::AccelerationGrid_t(const AccelerationGrid_t& other) :
        cell_res_(other.cell_res_), cell_size_(other.cell_size_), bounding_box_(other.bounding_box_), level_(other.level_), min_res_(other.min_res_), max_res_(other.max_res_), size_(other.size_) {
    cells_.reserve(other.cells_.size());
    for (const auto& cell: other.cells_) {
        cells_.push_back(std::make_unique<GridCell_t>(*cell));
    }
}

auto AccelerationGrid_t::operator=(const AccelerationGrid_t& other) -> AccelerationGrid_t& {
    if (this != &other) {
        *this = AccelerationGrid_t(other);
    }
    return *this;
}

auto AccelerationGrid_t::intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    double tbbox = 0.0;
    std::array<long long, 3> cellexit{0, 0, 0};
    std::array<int, 3> cellstep{0, 0, 0};
    const std::array<unsigned int, 8> map{2, 1, 2, 1, 2, 2, 0, 0};

    t                  = std::numeric_limits<double>::max();
    const Vec3f invdir = Vec3f(1.0) / ray.direction_;

    if (!bounding_box_.intersection(ray, tbbox)) {
        return nullptr;
    }

    Vec3f deltat = Vec3f();
    Vec3f tnext  = Vec3f();

    const Vec3f raycellorigin = (ray.origin_ + ray.direction_ * tbbox) - bounding_box_.coordinates_[0];
    const Vec3f cell_res      = Vec3f(static_cast<double>(cell_res_[0]), static_cast<double>(cell_res_[1]), static_cast<double>(cell_res_[2]));
    const Vec3f cellcoord     = (raycellorigin / cell_size_).floor().max(0.0).min(cell_res - 1.0);

    for (unsigned int i = 0; i < 3; ++i) {
        if (ray.direction_[i] < 0) {
            deltat[i]   = -cell_size_[i] * invdir[i];
            tnext[i]    = tbbox + (cellcoord[i] * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = -1;
            cellstep[i] = -1;
        }
        else {
            deltat[i]   = cell_size_[i] * invdir[i];
            tnext[i]    = tbbox + ((cellcoord[i] + 1.0) * cell_size_[i] - raycellorigin[i]) * invdir[i];
            cellexit[i] = static_cast<long long>(cell_res_[i]);
            cellstep[i] = 1;
        }
    }

    std::array<int, 3> cellcoordint{static_cast<int>(cellcoord[0]), static_cast<int>(cellcoord[1]), static_cast<int>(cellcoord[2])};
    Shape_t* hit_obj = nullptr;
    while (true) {
        if (cells_[cellcoordint[0] + cellcoordint[1] * cell_res_[0] + cellcoordint[2] * cell_res_[0] * cell_res_[1]]) {
            hit_obj = cells_[cellcoordint[0] + cellcoordint[1] * cell_res_[0] + cellcoordint[2] * cell_res_[0] * cell_res_[1]]->intersect(ray, t, uv);
        }

        const int k                 = static_cast<int>(tnext[0] < tnext[1]) * 4 + static_cast<int>(tnext[0] < tnext[2]) * 2 + static_cast<int>(tnext[1] < tnext[2]);
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

auto AccelerationGrid_t::add(Shape_t* item) -> void {
    Vec3f min1 = Vec3f(std::numeric_limits<double>::max());
    Vec3f max1 = Vec3f(std::numeric_limits<double>::lowest());

    min1.min(item->mincoord());
    max1.max(item->maxcoord());
    min1 = (min1 - bounding_box_.coordinates_[0]).floor() / cell_size_;
    max1 = (max1 - bounding_box_.coordinates_[0]).floor() / cell_size_;
    min1.max(0.0);
    max1.max(0.0);

    const Vec3f cell_res = Vec3f(static_cast<double>(cell_res_[0]), static_cast<double>(cell_res_[1]), static_cast<double>(cell_res_[2])) - 1.0;
    min1.min(cell_res);
    max1.min(cell_res);

    for (auto z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
        for (auto y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
            for (auto x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                if (!cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]) {
                    cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]] = std::make_unique<GridCell_t>();
                }
                cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]->add(item);
            }
        }
    }
    ++size_;
}

auto AccelerationGrid_t::remove(Shape_t* const item) -> void {
    Vec3f min1 = Vec3f(std::numeric_limits<double>::max());
    Vec3f max1 = Vec3f(std::numeric_limits<double>::lowest());

    min1.min(item->mincoord());
    max1.max(item->maxcoord());
    min1 = (min1 - bounding_box_.coordinates_[0]).floor() / cell_size_;
    max1 = (max1 - bounding_box_.coordinates_[0]).floor() / cell_size_;
    min1.max(0.0);
    max1.max(0.0);

    const Vec3f cell_res = Vec3f(static_cast<double>(cell_res_[0]), static_cast<double>(cell_res_[1]), static_cast<double>(cell_res_[2])) - 1.0;
    min1.min(cell_res);
    max1.min(cell_res);

    for (auto z = static_cast<size_t>(min1[2]); z <= static_cast<size_t>(max1[2]); ++z) {
        for (auto y = static_cast<size_t>(min1[1]); y <= static_cast<size_t>(max1[1]); ++y) {
            for (auto x = static_cast<size_t>(min1[0]); x <= static_cast<size_t>(max1[0]); ++x) {
                if (cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]) {
                    cells_[x + y * cell_res_[0] + z * cell_res_[0] * cell_res_[1]]->remove(item);
                }
            }
        }
    }
    --size_;
}

auto AccelerationGrid_t::move(Shape_t* item) -> void {}

auto AccelerationGrid_t::size() const -> size_t {
    return size_;
}

auto AccelerationGrid_t::clone() const -> std::unique_ptr<AccelerationStructure_t> {
    return std::make_unique<AccelerationGrid_t>(*this);
}
