#pragma once
#include "shape.h"
#include "shader.h"
#include <vector>
//Base (or Abstract) class, having derived classes triangle and sphere

class Triangle : public Shape{
    public:
        Triangle(point3 a, point3 b, point3 c) : pt_a(a), pt_b(b), pt_c(c), col(vec3(1.0f,1.0f,1.0f)), material(nullptr),vertexSet({a,b,c}) {}
        Triangle(point3 a, point3 b, point3 c, color col ) : pt_a(a), pt_b(b), pt_c(c), col(col), material(nullptr),vertexSet({a,b,c}) {}
        Triangle(point3 a, point3 b, point3 c, color col, std::shared_ptr<Shader> material) : pt_a(a), pt_b(b), pt_c(c), col(col), material(material),vertexSet({a,b,c}) {}
        bool intersect(const ray &ray,const float tmin, float &tmax, HitStruct& rec) override;

        vec3 getColor() const override;
        std::shared_ptr<Shader> getShader() const override;
        std::vector<float> extractVBO() override;
    private:
        std::shared_ptr<Shader> material;
        vec3 col;
        point3 pt_a;
        point3 pt_b;
        point3 pt_c;
        std::vector<vec3> vertexSet;
};