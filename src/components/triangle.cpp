#include "triangle.h"

bool Triangle::intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) {
    point3 eye = ray.origin();
    vec3 dir = ray.direction();

    double a = pt_a.x()-pt_b.x();
    double b = pt_a.y()-pt_b.y();
    double c = pt_a.z()-pt_b.z();
    double d = pt_a.x()-pt_c.x();
    double e = pt_a.y()-pt_c.y();
    double f = pt_a.z()-pt_c.z();
    double g = dir.x();
    double h = dir.y(); 
    double i = dir.z();
    double j = pt_a.x()-eye.x();
    double k = pt_a.y()-eye.y();
    double l = pt_a.z()-eye.z();

    double M = a*(e*i - h*f)+b*(g*f-d*i)+c*(d*h-e*g);
    double beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
    double gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
    double t = -(f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;

    if(t<tmin || t>tmax) {
        return false;
    }
    if(gamma < 0 || gamma > 1){
        return false;
    }
    if(beta <0 || beta > 1 -gamma) {
        return false;
    }

    tmax = t;
    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.normal = unit_vector(cross(pt_b-pt_a,pt_c-pt_a));

    return true;
} 
