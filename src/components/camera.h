#pragma once
#include "vec3.h"
#include "ray.h"

class Camera {
    public:
        std::string type;
        Camera(): pos(0.0f,0.0f,0.0f), U(1.0f,0.0f,0.0f), V(0.0f,1.0f,0.0f), W(0.0f,0.0f,1.0f), focalLength(0.0325f), imageplane_width(.5f), imageplane_height(.5f), nx(100), ny(100) {};
        Camera(int pixel_nx, int pixel_ny): pos(0.0f,0.0f,0.0f), U(1.0f,0.0f,0.0f), V(0.0f,1.0f,0.0f), W(0.0f,0.0f,1.0f), focalLength(0.0325f), imageplane_width(.5f), imageplane_height(.5f), nx(pixel_nx), ny(pixel_ny) {}
        Camera(point3 pos, vec3 direction, float focalLength, float imageplane_width, int pixel_nx, int pixel_ny) 
            : pos(pos),focalLength(focalLength), nx(pixel_nx), ny(pixel_ny) {
                // imageplane_width = 0.25;
                float aspect_ratio = (float)nx/(float)ny;
                imageplane_height = imageplane_width/aspect_ratio;
                l = -imageplane_width * 0.5f;
                r =  imageplane_width * 0.5f;
                b = -imageplane_height * 0.5f;
                t =  imageplane_height * 0.5f;
            }
        virtual ~Camera() {}

        virtual ray generateRay(float i, float j) = 0;

    protected:
        vec3 pos;
        
        //basis
        vec3 U,V,W;

        //We have to decide the unit
        float focalLength; //also called d
        int nx, ny;
        float imageplane_width, imageplane_height;
        
        float l, r, b, t;
};