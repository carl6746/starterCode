#include <iostream>
#include "Framebuffer.h"

int main() {
    std::cout<<"Hello World"<<std::endl;
    Framebuffer fb1;
    vec3 c1(1.0,0.65,0.98);
    fb1.clearToColor(c1);

    fb1.exportToPNG("../src/myfile.png");

    Framebuffer fb2(100,100);
    vec3 c2(.25,.5,.75);
    fb2.clearToColor(c2);
    fb2.exportToPNG("../src/myfile2.png");

    Framebuffer fb3(250,250);
    vec3 cbottom(241/255,187/255,51/255);
    vec3 ctop(128/255,7/255,241/255);
    fb3.clearToGradient(c1,c2);
    fb3.exportToPNG("../src/myGradientFile.png");

    return 0;
}

//We can use his cmake to target this