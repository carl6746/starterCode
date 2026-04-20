#include "sphere.h"

bool Sphere::intersect(const ray &ray,const float tmin, float &tmax, HitStruct& rec) {
    point3 e = ray.origin();
    vec3 d = ray.direction();

    float A = dot(d,d);
    float B = 2*dot(d,e-center);
    float C = dot(e-center,e-center)-radius*radius;

    float descriminant = B*B-4*A*C;
    if(descriminant < 0.0f) {
        return false;
    }

    float t = (-B - sqrt(descriminant))/(2.0f*A);
    if(t < tmin || t > tmax) {
        t = (-B + sqrt(descriminant))/(2.0f*A);
        if( t <tmin || t> tmax) {
            return false;
        }
    }

    tmax = t;
    rec.r = ray;
    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.normal = unit_vector(rec.p-center);
    rec.material = this->material;

    return true;
}

vec3 Sphere::getColor() const{
    return col;
}

std::shared_ptr<Shader> Sphere::getShader() const{
    return material;
}

std::vector<float> Sphere::extractVBO() {
    return std::vector({1.0f});
}

