#include "sphere.h"

bool Sphere::intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) {
    point3 e = ray.origin();
    vec3 d = ray.direction();

    double A = dot(d,d);
    double B = 2*dot(d,e-center);
    double C = dot(e-center,e-center)-radius*radius;

    double descriminant = B*B-4*A*C;
    if(descriminant < 0) {
        return false;
    }

    double t = (-B - sqrt(descriminant))/(2.0*A);
    if(t < tmin || t > tmax) {
        t = (-B + sqrt(descriminant))/(2.0*A);
        if( t <tmin || t> tmax) {
            return false;
        }
    }

    tmax = t;
    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.normal = unit_vector(rec.p-center);

    return true;
}
