#pragma once
#include <map>
#include "Framebuffer.h"
#include "perspectiveCamera.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
// #include "components/shader.h"
#include "lambert.h"
#include "blinn.h"
#include "mirror.h"
#include "normal.h"

class Scene {
    public:
        Scene(std::vector<std::shared_ptr<Shape>> shapes,std::vector<vec3> lightList) : shapes(shapes), lightList(lightList) {};
        color computeRayColor(const ray &r, double tmin, double tmax, int depth) {
            if(depth<=0) {
                return bgColor;
            }

            HitStruct hit;
            HitStruct shadowHit;
            double localTmax = tmax;
            double shadowTmax = 1.0;
           
            bool hitShape = false;

            for(size_t idx=0; idx<shapes.size(); ++idx) {
                if(shapes[idx]->intersect(r,tmin,localTmax,hit)) {
                    hitShape = true;
                    localTmax = hit.t;
                }
            }
            
            

            if(hitShape) {
                bool inShadow = false;
                for(size_t idx=0; idx<shapes.size(); ++idx) {
                    ray shadowRay(hit.p,lightList[0]-hit.p);
                    if(shapes[idx]->intersect(shadowRay,0.001,shadowTmax,shadowHit)) {
                        inShadow = true;
                    }
                }

                if(hit.material->is_mirror()) {
                    vec3 reflected = unit_vector(r.direction() - 2 * dot(r.direction(), hit.normal) * hit.normal);
                    ray reflected_ray(hit.p + 0.001 * hit.normal, reflected);
                    return computeRayColor(reflected_ray, 0.001, INFINITY, depth - 1);
                } else {
                    std::shared_ptr<Shader> shader = hit.material;
                    color c = shader->rayColor(hit,depth-1,inShadow);
                    return c;
                }
                
        } else {
            return bgColor;
        }
    }
    private:
        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<vec3> lightList;
        color bgColor = {0.0,0.0,0.0};
        
};