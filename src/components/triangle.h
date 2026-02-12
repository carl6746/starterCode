#pragma once

#include "shape.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Triangle : public Shape{
    public:
        Triangle(point3 a, point3 b, point3 c, color rgb) : pt_a(a), pt_b(b), pt_c(c), rgb(rgb) {}
        bool intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) override;
        color getColor() {
            return rgb;
        };
    private:
        color rgb;
        point3 pt_a;
        point3 pt_b;
        point3 pt_c;
};