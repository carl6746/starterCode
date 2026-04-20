#pragma once
#include <map>
#include "Framebuffer.h"
#include "perspectiveCamera.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "light.h"
// #include "components/shader.h"
#include "lambert.h"
#include "blinn.h"
#include "mirror.h"
#include "normal.h"

class Scene {
    public:
        Scene() {};
            friend class SceneLoader;
        
        color computeRayColor(const ray &r, float tmin, float tmax, int depth) {
            if (lights.empty()) {
                std::cerr << "No lights in scene!\n";
            }

            if(depth<=0) {
                return bgColor;
            }

            HitStruct hit;
            HitStruct shadowHit;
            float localTmax = tmax;
            float shadowTmax = 1.0f;
           
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
                    ray shadowRay(hit.p,lights[0]->getPosition()-hit.p);
                    if(shapes[idx]->intersect(shadowRay,0.001f,shadowTmax,shadowHit)) {
                        inShadow = true;
                    }
                }

                if(hit.material) {
                    if(hit.material->is_mirror()) {
                        vec3 reflected = unit_vector(r.direction() - 2 * dot(r.direction(), hit.normal) * hit.normal);
                        ray reflected_ray(hit.p + 0.001f * hit.normal, reflected);
                        return computeRayColor(reflected_ray, 0.001f, INFINITY, depth - 1);
                    } else {
                        std::shared_ptr<Shader> shader = hit.material;
                        color c = shader->rayColor(hit,depth-1,inShadow);
                        return c;
                    }
                } else {
                    std::cerr << "Warning: hit.material is null. Using shape color instead.\n";
                    return color({1.0f,0.0f,0.0f});  // fallback to the shape's color
                }
        } else {
            return bgColor;
        }
    }

        std::shared_ptr<Camera> getCamera() {
            if (!cameras.empty()) return cameras[0];
            return nullptr;
        }
        

        std::vector<float> glPrepareToRasterize() {
            std::vector<float> totalVBO;

            for (auto& iter : shapes) {
                std::vector<float> vbo = iter->extractVBO();
                totalVBO.insert(totalVBO.end(), vbo.begin(), vbo.end());
            }

            return totalVBO;
        }
        
    private:
        std::vector<std::shared_ptr<Light>> lights;
        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<std::shared_ptr<Camera>> cameras;
        color bgColor = {1.0f,1.0f,1.0f};
        
};