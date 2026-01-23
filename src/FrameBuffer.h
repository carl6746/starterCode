#pragma once

#include <vector>
#include "vec3.h"

class Framebuffer {
    public:
    Framebuffer();

    Framebuffer(int w, int h);

    void clearToColor(vec3 color);

    void exportToPNG(std::string filename);

    void setPixelColor(int i, int j, vec3 color);
    void setPixelColor(int index, vec3 color);

    private:
        int width, height;
        std::vector<vec3> fbStorage;
};