#include <iostream>
#include "components/Framebuffer.h"
#include "components/perspectiveCamera.h"
#include "components/sphere.h"

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
    double focallength=1;
    Framebuffer fb1(1000,1000);

    point3 center = {-1,-1,-100};
    Sphere s(center,1);
    point3 center1 = {1,1,-100};
    Sphere s1(center1,1);
    point3 center2 = {-1,1,-100};
    Sphere s2(center2,1);
    point3 center3 = {1,-1,-100};
    Sphere s3(center3,1);
    point3 center4 = {0,0,-90};
    Sphere s4(center4,1);


    std::vector<Sphere> SphereList = {s,s1,s2,s3,s4};


    PerspectiveCamera pc(origin, view, focallength, fb1.get_width(), fb1.get_height());

   
    fb1.clearToColor(color(1.0,0.0,0.0));
    for(int x = 0; x<fb1.get_width(); ++x) {
        for(int y =0; y<fb1.get_height(); ++y) {
            ray r = pc.generateRay(x,y);
                double tmin = 1;
                double tmax = INFINITY; 
            for(auto iter : SphereList) {
                if(iter.intersect(r,tmin,tmax)==true) {
                    fb1.setPixelColor(x,y,color(1.0,1.0,1.0));
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
    fb1.exportToPNG("../images/collisionTest.png");

    return 0;
}
