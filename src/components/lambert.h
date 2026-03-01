#pragma
#include "shader.h"
#include <vector>

class Lambert : public Shader {
    public:
        Lambert(const std::vector<vec3>& light,color c) {
            for(auto& l : light) {
                lightList.push_back(unit_vector(l));
            }
            rgb = c;
        };
        
        color rayColor( const HitStruct &h,int depth,bool inShadow) override {
            vec3 n = unit_vector(h.normal);
            double totalLight = 0.0;
            for(auto& l : lightList) {
                totalLight += std::max(0.0, dot(n, l));
            }
            if(inShadow) {
                return color({0,0,0});
            }
            return (rgb*totalLight);
        }

        point3 getLight() {
            return lightList[0];
        }

        bool is_mirror() override {
            return false;
        }
    private:
        std::vector<vec3> lightList;
        color rgb;
};