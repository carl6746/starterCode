#pragma
#include "shader.h"
#include <vector>

class Lambert : public Shader {
    public:
        Lambert(const std::vector<vec3>& light) {
            for(auto& l : light) {
                lightList.push_back(unit_vector(l));
            }
        };
        
        color rayColor( const HitStruct &h) override {
            vec3 n = unit_vector(h.normal);
            double totalLight = 0.0;
            for(auto& l : lightList) {
                totalLight += std::max(0.0, dot(n, l));
            }
            return (color(1.0,0.0,0.0)*totalLight);
        }
    private:
        std::vector<vec3> lightList;
};