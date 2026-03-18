#pragma once
#include "vec3.h"
#include "ray.h"

class Camera {
    public:
        std::string type;
        Camera(): pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,1), focalLength(0.0325), imageplane_width(.5), imageplane_height(.5), nx(100), ny(100) {};
        Camera(int pixel_nx, int pixel_ny): pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,1), focalLength(0.0325), imageplane_width(.5), imageplane_height(.5), nx(pixel_nx), ny(pixel_ny) {}
        Camera(point3 pos, vec3 direction, double focalLength, double imageplane_width, int pixel_nx, int pixel_ny) 
            : pos(pos),focalLength(focalLength), nx(pixel_nx), ny(pixel_ny) {
                // imageplane_width = 0.25;
                double aspect_ratio = (double)nx/(double)ny;
                imageplane_height = imageplane_width/aspect_ratio;
                l = -imageplane_width * 0.5;
                r =  imageplane_width * 0.5;
                b = -imageplane_height * 0.5;
                t =  imageplane_height * 0.5;
            }
        virtual ~Camera() {}

        virtual ray generateRay(double i, double j) = 0;

    protected:
        vec3 pos;
        
        //basis
        vec3 U,V,W;

        //We have to decide the unit
        double focalLength; //also called d
        int nx, ny;
        double imageplane_width, imageplane_height;
        
        double l, r, b, t;
};