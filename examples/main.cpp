#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <memory>
#include "components/scene.h"
#include "handleGraphicsArgs.h"
// #include "components/shader.h"

double randomOffSet() {
    static std::uniform_real_distribution<double> distribution(0.0,1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

int main(int argc, char *argv[]) {

    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    point3 origin = {0,0,0};
    vec3 view = {0,0,-1};
    double focallength = args.depthOfFieldDistance;
    Framebuffer fb1(args.width,args.height);

    HitStruct hit;
    PerspectiveCamera pc(origin, view, focallength, fb1.get_width(), fb1.get_height());

    std::vector<std::shared_ptr<Shape>> ShapeList;

    ShapeList.push_back(std::make_shared<Sphere>(point3(-20,15,-30),7,std::make_shared<Normal>()));
    ShapeList.push_back(std::make_shared<Sphere>(point3(20,15,-30),7,std::make_shared<Lambert>(std::vector<vec3>({vec3({0,10,10})}),vec3({0.75,0.2,0.7}))));
    ShapeList.push_back(std::make_shared<Sphere>(point3(0,5,-30),7,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    
    ShapeList.push_back(std::make_shared<Sphere>(point3(0,25,-30),7,std::make_shared<Mirror>()));

    // ShapeList.push_back(std::make_shared<Sphere>(point3(0,0,-50),15,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    ShapeList.push_back(std::make_shared<Triangle>(point3({-100,-10,-100}),point3({-100,-10,100}),point3({100,-10,-100}),std::make_shared<Lambert>(std::vector<vec3>({vec3({0,10,10})}),vec3({0.75,0.75,0.75}))));
    ShapeList.push_back(std::make_shared<Triangle>(point3({100,-10,100}),point3({100,-10,-100}),point3({-100,-10,100}),std::make_shared<Lambert>(std::vector<vec3>({vec3({0,10,10})}),vec3(0.75,0.75,0.75))));
    // ShapeList.push_back(std::make_shared<Sphere>(point3({-7,0,-20}),5,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    // ShapeList.push_back(std::make_shared<Sphere>(point3({0,7,-15}),5,std::make_shared<Mirror>()));
    // ShapeList.push_back(std::make_shared<Sphere>(point3({7,0,-10}),5,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    // // ShapeList.push_back(std::make_shared<Sphere>(point3({0,-10,-20}),5,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    // // ShapeList.push_back(std::make_shared<Sphere>(point3({0,10,-25}),5,std::make_shared<Blinn>(vec3({0,10,10}),vec3(0,0,0))));
    // ShapeList.push_back(std::make_shared<Triangle>(point3({-100,-10,-100}),point3({-100,-10,100}),point3({100,-10,-100}),std::make_shared<Lambert>(std::vector<vec3>({vec3({0,10,10})}),vec3({0.75,0.75,0.75}))));
    // ShapeList.push_back(std::make_shared<Triangle>(point3({100,-10,100}),point3({100,-10,-100}),point3({-100,-10,100}),std::make_shared<Lambert>(std::vector<vec3>({vec3({0,10,10})}),vec3(0.75,0.75,0.75))));

    
    Scene sc(ShapeList,std::vector<vec3>({vec3({0,10,10})}));
    int rpp_NSquare = 2;

    // #pragma omp parallel for
    for(int y = 0; y<fb1.get_height(); ++y) {
        for(int x =0; x<fb1.get_width(); ++x) {
            
            color c(0.0,0.0,0.0);

            for(int p=0; p<rpp_NSquare; ++p) {
                for(int q=0; q<rpp_NSquare; ++q) {
                    
                    double tmin = 1;
                    double tmax = std::numeric_limits<double>::infinity(); 

                    double pOffSet = (p + randomOffSet())/rpp_NSquare;
                    double qOffSet = (q + randomOffSet())/rpp_NSquare;

                    ray r = pc.generateRay(x+p,y+q);
                    c += sc.computeRayColor(r,tmin,tmax,3);
                }
            }
            c /= (rpp_NSquare * rpp_NSquare);
            fb1.setPixelColor(x,y,c);
        }
    }

    std::cout<<"Made it out of loops"<<std::endl;

    fb1.exportToPNG("../images/MultipleShaders.png");
    std::cout << "Image saved" <<std::endl;
    return 0;
}
