#pragma once
#include "../vec3.h"

class Camera {
    public:
        Camera(): pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,1), focalLength(1.0), imageplane_width(0.25), imageplane_height(0.25) {}
        Camera(vec3 pos, vec3 U, vec3 V, vec3 W, double focalLength, double imageplane_width, double imageplane_height) 
            : pos(pos), U(U), V(V), W(W),focalLength(focalLength), imageplane_width(imageplane_width), imageplane_height(imageplane_height) {}
        virtual ~Camera() {}

    protected:
        vec3 pos;
        
        //basis
        vec3 U,V,W;

        //We have to decide the unit
        double focalLength; //also called d

        double imageplane_width, imageplane_height;
};