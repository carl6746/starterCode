

#pragma once
#include "shape.h"
#include <memory>

class bvhNode : public Shape {
    public:
    bvhNode() {};
    ~bvhNode() {};

    vec3 centroid(std::shared_ptr<Shape>) {
        
    }

    bool intersect(const ray &ray, const double tmin, double &tmax, HitStruct& rec) override {

    };

    private:
};