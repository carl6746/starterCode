#pragma once
#include "shader.h"
#include <vector>
#include <algorithm>

class Blinn : public Shader {
    public:
        Blinn(const point3 light, point3 e, color c) : light(light),view(e), rgb(c) {};

        color getShaderColor() {
            return rgb;
        }
        
        color rayColor( const HitStruct &h, int depth, bool inShadow) override {
            vec3 n = unit_vector(h.normal);
            vec3 v = unit_vector(view-h.p);
            vec3 lDir = unit_vector(light); 

            vec3 hVec = unit_vector(light+v);

            float totalLight = 0.0f;

            float phongExp = 64.0f;

            
            vec3 spec = color({1.0,0.0,0.0})*std::max(0.0f, dot(n,lDir)) + color({1.0f,1.0f,1.0f})*std::pow(std::max(0.0f,dot(n,hVec)),phongExp);
            vec3 clampedSpec({std::clamp(spec[0],0.0f,1.0f),std::clamp(spec[1],0.0f,1.0f),std::clamp(spec[2],0.0f,1.0f)});
            return  clampedSpec; 
        }

        point3 getLight() {
            return light;
        }

        bool is_mirror() override {
            return false;
        }
    private:
        vec3 light;
        point3 view;
        color rgb;
};