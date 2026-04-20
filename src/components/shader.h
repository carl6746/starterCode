#pragma once
#include "vec3.h"
#include "shape.h"


struct Hitstruct;

class Shader {
    public:
        virtual color rayColor(const HitStruct &h,int depth, bool inShadow) = 0;
        virtual color getShaderColor() = 0;
        virtual point3 getLight() = 0;
        virtual bool is_mirror() = 0;
    protected:

};