#pragma once

#include "shape.h"
#include "shader.h"
//Base (or Abstract) class, having derived classes triangle and sphere

class Sphere : public Shape{
    public:
        Sphere(point3 c, double r) : center(c),radius(r), col(vec3(1.0,1.0,1.0)), material(nullptr) {}
        Sphere(point3 c, double r, color col) : center(c),radius(r), col(col), material(nullptr) {}
        Sphere(point3 c, double r, color col, std::shared_ptr<Shader> material) : center(c),radius(r), col(col), material(material) {}
        
        bool intersect(const ray &ray,const double tmin, double &tmax, HitStruct& rec) override;
        vec3 getColor() const override;
        std::shared_ptr<Shader> getShader() const override;

    
    private:
        point3 center;
        double radius;
        std::shared_ptr<Shader> material;
        color col;
};

