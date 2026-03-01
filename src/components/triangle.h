#pragma once

#include "shape.h"
#include "shader.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Triangle : public Shape{
    public:
        Triangle(point3 a, point3 b, point3 c, std::shared_ptr<Shader> material ) : pt_a(a), pt_b(b), pt_c(c), material(material) {}
        bool intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) override;
    private:
        std::shared_ptr<Shader> material;
        point3 pt_a;
        point3 pt_b;
        point3 pt_c;
};