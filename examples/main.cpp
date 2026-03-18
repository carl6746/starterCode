#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <memory>
#include "components/scene.h"
#include "handleGraphicsArgs.h"
#include "components/shader.h"
#include "components/ISceneLoader.h"
#include "components/SceneLoader.h"
#include "components/SceneParser_JSON.cpp"

double randomOffSet() {
    static std::uniform_real_distribution<double> distribution(0.0,1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

int main(int argc, char* argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    Scene sc;
    
    std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(sc);
    SceneParser_JSON parser(loader);

    std::string filename= "C:/Users/ajcar/CS4212/starterCode/sceneData-main/scenes_A/LambertianChalkSpheres.json";;
    parser.parseFileData(filename);

    auto pc = sc.getCamera();
        if (!pc) {
            std::cerr << "No camera loaded!" << std::endl;
            return 1;
        }

    double focallength = args.depthOfFieldDistance;
    Framebuffer fb1(args.width,args.height);

    HitStruct hit;

    int rpp_NSquare = args.rpp;

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

                    ray r = pc->generateRay(x+p,y+q);
                    c += sc.computeRayColor(r,tmin,tmax,3);
                }
            }
            c /= (rpp_NSquare * rpp_NSquare);
            fb1.setPixelColor(x,y,c);
        }
    }

    std::cout<<"Made it out of loops"<<std::endl;

    fb1.exportToPNG("../images/JSONTest.png");
    std::cout << "Image saved" <<std::endl;
    return 0;
}


