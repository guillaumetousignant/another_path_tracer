#ifndef ACCELERATIONSTRUCTURE_T_H
#define ACCELERATIONSTRUCTURE_T_H

#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;

    /**
     * @brief The acceleration structure class holds shapes and can be intersected.
     * 
     * This class is what is queried to find what shape a ray intersects. It is an interface
     * class, defining what a concrete acceleration structure must implement. An acceleration
     * structure should accelerate ray intersection compared to a brute-force approach.
     */
    class AccelerationStructure_t{
        public:
            /**
             * @brief Construct a new AccelerationStructure_t object. Does nothing.
             */
            AccelerationStructure_t(){};

            /**
             * @brief Destroy the AccelerationStructure_t object. Does nothing.
             */
            virtual ~AccelerationStructure_t(){};

            size_t n_obj_; /**< @brief Number of shapes currently held in the acceleration structure.*/

            /**
             * @brief Intersects a ray with the acceleration structure to find the closest hit
             * and its caracteristics.
             * 
             * Main function of the acceleration structure. Virtual function, it should return 
             * the information of the closest intersection, or nullptr if no intersection. t 
             * and uv may have undefined values if no intersection is found.
             * 
             * @param[in] ray Ray to be intersected with the acceleration structure, using its current origin and direction.
             * @param[out] t Distance to intersection. It is stored in t if there is an intersection.
             * @param[out] uv 2D object-space coordinates of the intersection.
             * @return Shape_t* Pointer of the intersected shape. Returns nullptr if there is no intersection.
             */
            virtual Shape_t* intersect(const Ray_t &ray, double &t, double (&uv)[2]) const = 0;
            
            /**
             * @brief Adds a shape to the acceleration structure.
             * 
             * Virtual function to add a shape to the acceleration structure. Must be implemented to make multi-level
             * acceleration structures possible. Not all acceleration structures are capable of
             * inserting shapes without a refit, so this operation may be pricey and should be 
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             * 
             * @param[in] item Shape to be added to the acceleration structure.
             */
            virtual void add(Shape_t* item) = 0;

            /**
             * @brief Removes a shape from the acceleration structure.
             * 
             * Virtual function to remove a shape from the acceleration structure. Must be implemented to make moving objects
             * possible. Not all acceleration structures are capable of
             * removing shapes without a refit, so this operation may be pricey and should be 
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             * 
             * @param item Shape to be removed from the acceleration structure.
             */
            virtual void remove(const Shape_t* item) = 0;

            /**
             * @brief Moves a shape in the acceleration structure.
             * 
             * Virtual function to move a shape within the acceleration structure. Must be implemented to make moving objects
             * possible. Not all acceleration structures are capable of
             * moving shapes without a refit, so this operation may be pricey and should be 
             * considered when choosing a nested acceleration structure if it can't be constructed
             * all at once.
             * 
             * @param item Shape to be removed from the acceleration structure.
             */
            virtual void move(Shape_t* item) = 0;
    };
}}

#endif