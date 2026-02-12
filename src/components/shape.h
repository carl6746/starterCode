#pragma once

#include "ray.h"
//Base (or Abstract) class, having derived classes triangle and sphere
struct HitStruct {
    point3 p;
    vec3 normal;
    double t;   
};

class Shape {
    public:
        Shape() {}
        virtual ~Shape() {};
        virtual color getColor() = 0;
        virtual bool intersect(const ray &ray, const double tmin, double &tmax, HitStruct& rec) = 0;
    protected:
};

