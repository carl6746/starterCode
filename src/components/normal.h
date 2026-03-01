#pragma
#include "shader.h"
#include <vector>

class Normal : public Shader {
    public:
        Normal() {};
        
        color rayColor( const HitStruct &h,int depth,bool inShadow) override {
            vec3 n = unit_vector(h.normal);
            if(inShadow) {
                return color({0,0,0});
            }
            return n;
        }

        point3 getLight() {
            return vec3({0,0,0});
        }

        bool is_mirror() override {
            return false;
        }
    private:
};