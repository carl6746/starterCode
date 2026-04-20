#pragma
#include "shader.h"
#include <vector>

class Normal : public Shader {
    public:
        Normal() {};
        
        color rayColor( const HitStruct &h,int depth,bool inShadow) override {
            vec3 n = unit_vector(h.normal);
            if(inShadow) {
                return color({0.0f,0.0f,0.0f});
            }
            return n;
        }

        color getShaderColor() {
            return color({0.0f,0.0f,0.0f});
        }

        point3 getLight() {
            return vec3({0.0f,0.0f,0.0f});
        }

        bool is_mirror() override {
            return false;
        }
    private:
};