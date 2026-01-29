#include <iostream>
#include "components/Framebuffer.h"

int main() {
    std::cout<<"Hello World"<<std::endl;
    Framebuffer fb1;
    vec3 c1(1.0,0.65,0.98);
    fb1.clearToColor(c1);

    fb1.exportToPNG("../images/myfile.png");

    Framebuffer fb2(100,100);
    vec3 c2(.25,.5,.75);
    fb2.clearToColor(c2);
    fb2.exportToPNG("../images/myfile2.png");

    Framebuffer fb3(250,250);
    vec3 cbottom(0.94,0.73,0.20);
    vec3 ctop(0.50,0.03,0.94);
    vec3 cpurp(1.0,0.57,0.82);
    fb3.clearToGradient(cpurp,cbottom);
    fb3.exportToPNG("../images/myGradientFile.png");

    return 0;
}

//We can use his cmake to target this