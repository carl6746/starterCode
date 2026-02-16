#include <iostream>
#include <memory>
#include "components/Framebuffer.h"
#include "components/perspectiveCamera.h"
#include "components/sphere.h"
#include "components/triangle.h"
// #include "components/shader.h"
#include "components/lambert.h"
#include "components/blinn.h"
// #include "scene.h"

int main() {
    // point3 origin = {12.3,4.5,1.0};
    // vec3 view = {9.7,1.0,-2.3};
    // double focallength = 0.25;
    // double image_plane_width = 0.5;
    // double image_plane_height = 0.5;
    // Framebuffer fb1(200,200);

    // point3 origin = {5,-5,3};
    // vec3 view = {0,1,-1};
    // double focallength = 0.05;
    // double image_plane_width = 0.5;
    // double image_plane_height = 0.5;
    // Framebuffer fb1(200,200);

    point3 origin = {0,0,0};
    vec3 view = {0,0,-1};
    double focallength = 1;
    Framebuffer fb1(1000,1000);

std::vector<std::shared_ptr<Shape>> SphereListLambert;
std::vector<std::shared_ptr<Shape>> SphereListBlinn;

SphereListLambert.push_back(std::make_shared<Sphere>(point3({-6,-6,-100}), 2, color({1.0,0.0,0.0})));
SphereListLambert.push_back(std::make_shared<Sphere>(point3({-2,-2,-100}), 2, color({0.0,1.0,0.0})));
SphereListLambert.push_back(std::make_shared<Sphere>(point3({-6,-2,-100}), 2, color({0.0,0.0,1.0})));
SphereListLambert.push_back(std::make_shared<Sphere>(point3({-2,-6,-100}), 2, color({0.0,0.0,0.0})));
SphereListLambert.push_back(std::make_shared<Sphere>(point3({-3,-3,-75}), 1.5, color({0.0,0.0,0.0})));
SphereListBlinn.push_back(std::make_shared<Sphere>(point3({2,2,-100}), 2, color({1.0,0.0,0.0})));
SphereListBlinn.push_back(std::make_shared<Sphere>(point3({6,6,-100}), 2, color({0.0,1.0,0.0})));
SphereListBlinn.push_back(std::make_shared<Sphere>(point3({2,6,-100}), 2, color({0.0,0.0,1.0})));
SphereListBlinn.push_back(std::make_shared<Sphere>(point3({6,2,-100}), 2, color({0.0,0.0,0.0})));
SphereListBlinn.push_back(std::make_shared<Sphere>(point3({3,3,-75}), 1.5, color({0.0,0.0,0.0})));
// SphereList.push_back(std::make_shared<Triangle>(point3{0.0,0.0,-75.0}, point3{0,5.0,-70.0}, point3{5.0,0,-70.0},color({0.0,0.0,0.0})));
// SphereList.push_back(std::make_shared<Triangle>(point3{0.0,0.0,-75.0}, point3{0,5.0,-70.0}, point3{-5.0,0,-70.0},color({0.0,0.0,0.0})));
// SphereList.push_back(std::make_shared<Triangle>(point3{0.0,0.0,-75.0}, point3{0,-5.0,-70.0}, point3{5.0,0,-70.0},color({0.0,0.0,0.0})));
// SphereList.push_back(std::make_shared<Triangle>(point3{0.0,0.0,-75.0}, point3{0,-5.0,-70.0}, point3{-5.0,0,-70.0},color({0.0,0.0,0.0})));

    PerspectiveCamera pc(origin, view, focallength, fb1.get_width(), fb1.get_height());

   
    fb1.clearToColor(color(1.0,1.0,1.0));
    HitStruct hit;

    std::vector<vec3> lightList = {vec3{2.0,10.0,10.0}};
    Lambert lamb(lightList);

    Blinn blinnPhong(vec3{2.0,10.0,10.0},origin);

    

    for(int x = 0; x<fb1.get_width(); ++x) {
        for(int y =0; y<fb1.get_height(); ++y) {
            ray r = pc.generateRay(x,y);
                double tmin = 1;
                double tmax = 500.0; 
            for(auto& iter : SphereListLambert) {
                if(iter->intersect(r,tmin,tmax,hit)==true) {
                    vec3 ray_direction_color = 0.5*(unit_vector(hit.normal) + vec3(1,1,1));
                    fb1.setPixelColor(x,y,lamb.rayColor(hit));
                    // fb1.setPixelColor(x,y,iter->getColor());
                }
            }
            for(auto& iter : SphereListBlinn) {
                if(iter->intersect(r,tmin,tmax,hit)==true) {
                    vec3 ray_direction_color = 0.5*(unit_vector(hit.normal) + vec3(1,1,1));
                    fb1.setPixelColor(x,y,blinnPhong.rayColor(hit));
                    // fb1.setPixelColor(x,y,iter->getColor());
                }
               
            }

        }
    }
    // for(int x = 0; x<fb1.get_width(); ++x) {
    //     for(int y =0; y<fb1.get_height(); ++y) {
    //         ray r = pc.generateRay(x,y);
    //         vec3 ray_direction_color = 0.5*(unit_vector(r.direction()) + vec3(1,1,1));

    //         fb1.setPixelColor(x,y,ray_direction_color);
    //     }
    // }
    fb1.exportToPNG("../images/normalShaderTest.png");

    return 0;
}
