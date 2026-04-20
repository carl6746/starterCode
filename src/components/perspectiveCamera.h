#pragma once

#include "ray.h"
#include "camera.h"

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera() : Camera() {};
        PerspectiveCamera(point3 pos, vec3 direction, float focalLength, float imageplane_width, int pixel_nx, int pixel_ny) : Camera( pos,  direction,  focalLength, imageplane_width, pixel_nx,  pixel_ny) {
            create_basis(direction);
        };

        float u,v;
        ray generateRay(float i, float j) override {
            u = l + (r-l)*(i+(float)rand()/(float)RAND_MAX)/nx;
            v = b + (t-b)*(j+(float)rand()/(float)RAND_MAX)/ny;

            ray tempRay(pos, -W*focalLength+U*u+V*v);

            return tempRay;
        }

        void create_basis(vec3 direction) {
            vec3 b = {0.0f,1.0f,0.0f};

            W = -unit_vector(direction);
            U = (cross(b,W))/cross(b,W).length();
            V = cross(W,U);
        }
    private:
};