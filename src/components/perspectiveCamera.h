#pragma once

#include "ray.h"
#include "camera.h"

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera() : Camera(100,100) {};
        PerspectiveCamera(point3 pos, vec3 direction, double focalLength, int pixel_nx, int pixel_ny) : Camera( pos,  direction,  focalLength, pixel_nx,  pixel_ny) {
            create_basis(direction);
        };

        double u,v;
        ray generateRay(float i, float j) override {
            u = l + (r-l)*(i+0.5)/nx;
            v = b + (t-b)*(j+0.5)/ny;

            ray tempRay(pos, -W*focalLength+U*u+V*v);

            return tempRay;
        }

        void create_basis(vec3 direction) {
            vec3 b = {0,1,0};

            W = -unit_vector(direction);
            U = (cross(b,W))/cross(b,W).length();
            V = cross(W,U);
        }
    private:
};