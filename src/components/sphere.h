#pragma once

#include "shape.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Sphere : public Shape{
    public:
        Sphere(point3 c, double r) : center(c),radius(r) {}
        bool intersect(const ray &ray,const double tmin, double &tmax) override;
    private:
        point3 center;
        double radius;
};

