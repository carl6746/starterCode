#pragma once

#include "ray.h"
//Base (or Abstract) class, having derived classes triangle and sphere
class Shader;

struct HitStruct {
    point3 p;
    vec3 normal;
    double t;   
    ray r;
    std::shared_ptr<Shader> material;
};

class Shape {
    public:
        Shape() {}
        virtual ~Shape() {};
        virtual bool intersect(const ray &ray, const double tmin, double &tmax, HitStruct& rec) = 0;
    protected:
};

