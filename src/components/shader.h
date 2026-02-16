#pragma once
#include "vec3.h"
#include "shape.h"

class Shader {
    public:
        virtual color rayColor( const HitStruct &h) = 0;
    protected:

};