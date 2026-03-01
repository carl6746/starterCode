#pragma
#include "shader.h"
#include <vector>
#include <algorithm>

class Blinn : public Shader {
    public:
        Blinn(const point3 light, point3 e) : light(light),view(e) {};
        
        color rayColor( const HitStruct &h, int depth, bool inShadow) override {
            vec3 n = unit_vector(h.normal);
            vec3 v = unit_vector(view-h.p);
            vec3 lDir = unit_vector(light); 

            vec3 hVec = unit_vector(light+v);

            double totalLight = 0.0;

            double phongExp = 64;

            
            vec3 spec = color({1.0,0.0,0.0})*std::max(0.0, dot(n,lDir)) + color({1.0,1.0,1.0})*std::pow(std::max(0.0,dot(n,hVec)),phongExp);
            vec3 clampedSpec({std::clamp(spec[0],0.0,1.0),std::clamp(spec[1],0.0,1.0),std::clamp(spec[2],0.0,1.0)});
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
};