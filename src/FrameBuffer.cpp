#include "FrameBuffer.h"
#include <png++/png.hpp>
#include <algorithm>   // std::clamp

Framebuffer::Framebuffer()
    : width(100), height(100), fbStorage(width*height)
{

};

Framebuffer::Framebuffer(int w,int h)
    :  width(w), height(h), fbStorage(width*height)
{
    
};


void Framebuffer::clearToColor(vec3 color)
{
    for(auto idx =0u; idx<fbStorage.size(); ++idx){
        fbStorage[idx] = color;
    }
};

void Framebuffer::clearToGradient(vec3 bottom_color, vec3 top_color)
{
    for(auto idx=0u; idx<fbStorage.size(); ++idx){
        int y = idx/width;
        double p = double(y)/(height-1);
        fbStorage[idx] = p*bottom_color + (1-p)*top_color;
    }
};

void Framebuffer::exportToPNG(std::string filename)
{
    png::image<png::rgb_pixel> image(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            vec3 c = fbStorage[y * width + x];

            png::byte r = static_cast<png::byte>(std::clamp(c.x(), 0.0, 255.0)*255.0);
            png::byte g = static_cast<png::byte>(std::clamp(c.y(), 0.0, 255.0)*255.0);
            png::byte b = static_cast<png::byte>(std::clamp(c.z(), 0.0, 255.0)*255.0);

            image[y][x] = png::rgb_pixel(r, g, b);
        }
    }

    image.write(filename);
};

void Framebuffer::setPixelColor(int i, int j, vec3 color)
{
    if (i < 0 || i >= width || j < 0 || j >= height) return;
    fbStorage[j * width + i] = color;
};  

void Framebuffer::setPixelColor(int index, vec3 color)
{
    if (index < 0 || index >= static_cast<int>(fbStorage.size())) return;
    fbStorage[index] = color;
};
