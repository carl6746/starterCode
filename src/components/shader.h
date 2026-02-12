#pragma once
#include "vec3.h"
#include "shape.h"

class Shader {
    private:
        virtual color rayColor( const HitStruct &h) = 0;
    public:

};