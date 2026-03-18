#pragma once
#include "vec3.h"
//Base (or abstract) class


class Light {
    public:
        Light(const vec3& pos, const vec3& col, double intens = 1.0)
        : position(pos), color(col), intensity(intens) {}

        vec3 getPosition() const { return position; }
        vec3 getColor() const { return color; }
        double getIntensity() const { return intensity; }
        std::string type;
    private:
        vec3 position;
        vec3 color;
        double intensity = 1.0;
};