#pragma once

#include "shader.h"

class Emitter : public Shader {
    public:
    Emitter() {}

    color rayColor(const HitStruct &h,int depth,bool isShadow) override {};
    point3 getLight() override {};
    bool is_mirror() override {};

    private:
};