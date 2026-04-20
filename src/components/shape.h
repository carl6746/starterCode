#pragma once
#include <vector>
#include "ray.h"
//Base (or Abstract) class, having derived classes triangle and sphere
class Shader;

struct HitStruct {
    point3 p;
    vec3 normal;
    float t;   
    ray r;
    std::shared_ptr<Shader> material;
};

class Shape {
    public:
        std::string type;
        Shape() {}
        virtual ~Shape() {};
        virtual std::shared_ptr<Shader> getShader() const = 0;
        virtual color getColor() const = 0;
        virtual bool intersect(const ray &ray, const float tmin, float &tmax, HitStruct& rec) = 0;
        virtual std::vector<float> extractVBO() = 0;
    protected:
};

