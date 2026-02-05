#include "sphere.h"

bool Sphere::intersect(const ray &ray,const double tmin, double &tmax) {
    point3 e = ray.origin();
    vec3 d = ray.direction();

    double A = dot(d,d);
    double B = 2*dot(d,e-center);
    double C = dot(e-center,e-center)-radius*radius;

    if(B*B-4*A*C < 0) {
        return false;
    } else {
        return true;
    }
}
