#pragma once

#include "vec3.h"

class Ray {
    public:
        Ray() : orig(0.0,0.0,0.0), dir(0.0,0.0,-1.0) {}
        Ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
        ~Ray() {}

        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}

        point3 at(double t) const {
            return orig+t*dir;
        }

    private:
        point3 orig;
        vec3 dir;
};