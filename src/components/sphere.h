#pragma once

#include "shape.h"
#include "shader.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Sphere : public Shape{
    public:
        Sphere(point3 c, double r, std::shared_ptr<Shader> material) : center(c),radius(r), material(material) {}
        bool intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) override;
    private:
        point3 center;
        double radius;
        std::shared_ptr<Shader> material;
};

