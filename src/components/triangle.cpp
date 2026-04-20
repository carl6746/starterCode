#include "triangle.h"

bool Triangle::intersect(const ray &ray,const float tmin, float &tmax, HitStruct& rec) {
    point3 eye = ray.origin();
    vec3 dir = ray.direction();

    float a = pt_a.x()-pt_b.x();
    float b = pt_a.y()-pt_b.y();
    float c = pt_a.z()-pt_b.z();
    float d = pt_a.x()-pt_c.x();
    float e = pt_a.y()-pt_c.y();
    float f = pt_a.z()-pt_c.z();
    float g = dir.x();
    float h = dir.y(); 
    float i = dir.z();
    float j = pt_a.x()-eye.x();
    float k = pt_a.y()-eye.y();
    float l = pt_a.z()-eye.z();

    float M = a*(e*i - h*f)+b*(g*f-d*i)+c*(d*h-e*g);
    if (std::abs(M) < 1e-6f) {
        return false;
    }
    float beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
    float gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
    float t = -(f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;

    if(t<=tmin || t>=tmax) {
        return false;
    }
    if(gamma < 0.0f || gamma > 1.0f){
        return false;
    }
    if(beta <0.0f || beta > 1.0f -gamma) {
        return false;
    }

    tmax = t;
    rec.r = ray;
    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.normal = unit_vector(cross(pt_c-pt_a,pt_b-pt_a));
    rec.material = this->material;

    return true;
} 

vec3 Triangle::getColor() const{
    return material->getShaderColor();
}

std::shared_ptr<Shader> Triangle::getShader() const{
    return material;
}

std::vector<float> Triangle::extractVBO() {
    vec3 norm;
    norm = unit_vector(cross(pt_b-pt_a,pt_c-pt_a));
    std::vector<float> vertexVBO;
    for(auto iter : vertexSet) {
        vertexVBO.push_back(iter[0]);
        vertexVBO.push_back(iter[1]);
        vertexVBO.push_back(iter[2]);
        vertexVBO.push_back(norm[0]);
        vertexVBO.push_back(norm[1]);
        vertexVBO.push_back(norm[2]);
        vertexVBO.push_back(getColor()[0]);
        vertexVBO.push_back(getColor()[1]);
        vertexVBO.push_back(getColor()[2]);
        std::cout<<"Color: " << getColor()[0]<<", "<< getColor()[1]<<", "<<getColor()[2]<<std::endl;
    }
    return vertexVBO;
}

