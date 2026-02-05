#pragma once

#include "ray.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Shape {
    public:
        Shape() {}
        virtual ~Shape() {};

        virtual bool intersect(const ray &ray, const double tmin, double &tmax) = 0;
    protected:
};

