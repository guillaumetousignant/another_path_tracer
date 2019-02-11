#include "Sphere_t.h"
#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Material_t.h"

int main(){
    Material_t* material = new Material_t();
    Ray_t* ray = new Ray_t();
    TransformMatrix_t* transform_matrix = new TransformMatrix_t();

    Sphere_t* sphere = new Sphere_t(material, transform_matrix);
    return 0;
}