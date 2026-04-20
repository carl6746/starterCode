#pragma once

#include "vec3.h"

class ray {
    public:
        ray() : orig(0.0f,0.0f,0.0f), dir(0.0f,0.0f,-1.0f) {}
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
        ~ray() {}

        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}

        point3 at(float t) const {
            return orig+t*dir;
        }

    private:
        point3 orig;
        vec3 dir;
};