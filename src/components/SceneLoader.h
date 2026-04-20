#pragma once
#include <unordered_map>
#include "ISceneLoader.h"
#include "Scene.h"

class SceneLoader : public ISceneLoader {
private:
  Scene &m_targetScene; // Reference to the external scene
  std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
public:
  // The caller provides the scene to be filled
  SceneLoader(Scene &sceneToPopulate)
      : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    m_targetScene.cameras.reserve(count);
  }
  void reserveLights(size_t count) override {
    m_targetScene.lights.reserve(count);
  }
  void reserveShapes(size_t count) override {
    m_targetScene.shapes.reserve(count);
  }
  void reserveShaders(size_t count) override { numShaders = count; }
  // void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;
  }

  void addCamera(const std::string &name, const std::string &type,
               const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
               float focalLength, float imagePlaneWidth) override {
    std::cout << "Creating camera: " << name << ", type:" << type << std::endl;

    int nx = 1000; 
    int ny = 1000;

    vec3 camPos(pos.x, pos.y, pos.z);
    vec3 camDir(viewDir.x, viewDir.y, viewDir.z);
    std::cout << "Camera Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    std::cout << "Camera View Direction: (" << viewDir.x << ", " << viewDir.y << ", " << viewDir.z << ")\n";
    auto cam = std::make_shared<PerspectiveCamera>(camPos, camDir, focalLength, imagePlaneWidth,nx,ny);
    cam->type = type;

    m_targetScene.cameras.push_back(cam);
 }
  void addPointLight(const ISceneLoader::vec &pos,
                     const ISceneLoader::vec &intensity) override {
    std::cout << "Creating point light." << std::endl;
    vec3 position(pos.x, pos.y, pos.z);
    vec3 color(intensity.x, intensity.y, intensity.z);

    auto light = std::make_shared<Light>(position, color);
    light->type = "Point Light";
    m_targetScene.lights.push_back(light);
  }
  void addAreaLight(const ISceneLoader::vec &pos,
                    const ISceneLoader::vec &color,
                    const ISceneLoader::vec &normal, float width,
                    float length) override {
    std::cout << "Creating area light." << std::endl;
  }
  void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;

    vec3 diffuse(shaderDesc.diffuse.data.x,
                 shaderDesc.diffuse.data.y,
                 shaderDesc.diffuse.data.z);

    std::shared_ptr<Shader> s;

    if (shaderDesc.type == "Lambertian") {
        std::vector<vec3> lightPositions;
        for (auto &light : m_targetScene.lights) {
            lightPositions.push_back(light->getPosition());
        }

        s = std::make_shared<Lambert>(lightPositions, diffuse);
    }
    if (shaderDesc.type == "BlinnPhong") {

        s = std::make_shared<Blinn>(point3({0.0f,0.0f,0.0f}), point3({0.0f,0.0f,0.0f}), diffuse);
    }

    shaderMap[shaderDesc.name] = s;
}
  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

    // Determine the shader to use
    std::shared_ptr<Shader> s = nullptr;
    if (!shapeDesc.shaderNameReference.empty()) {
        auto it = shaderMap.find(shapeDesc.shaderNameReference); 
        if (it != shaderMap.end()) {
            s = it->second;  
            std::cout << "Using shader: " << shapeDesc.shaderNameReference << std::endl;
        } else {
            std::cerr << "Shader '" << shapeDesc.shaderNameReference 
                      << "' not found, using default Lambertian.\n";
        }
    }

    if (!s) {
        std::vector<vec3> lightPositions;
        for (auto &light : m_targetScene.lights) {
            lightPositions.push_back(light->getPosition());
        }
        s = std::make_shared<Lambert>(lightPositions, vec3(1.0f, 1.0f, 1.0f));
    }

    if (shapeDesc.type == "sphere") {
        point3 center(shapeDesc.center.x, shapeDesc.center.y, shapeDesc.center.z);
        float radius = shapeDesc.radius;

        m_targetScene.shapes.push_back(std::make_shared<Sphere>(center, radius, vec3(1.0f,1.0f,1.0f), s));
    }
    
    else if (shapeDesc.type == "triangle") {
        point3 v0(shapeDesc.v0.x, shapeDesc.v0.y, shapeDesc.v0.z);
        point3 v1(shapeDesc.v1.x, shapeDesc.v1.y, shapeDesc.v1.z);
        point3 v2(shapeDesc.v2.x, shapeDesc.v2.y, shapeDesc.v2.z);

        m_targetScene.shapes.push_back(std::make_shared<Triangle>(v0, v1, v2, vec3(1.0f,1.0f,1.0f), s));
    }
}
  // void addTexture(const std::string &type, const std::string &name,
  //                 const std::string &sourceFile) override {
  //   std::cout << "Creating texture: type=" << type << ", name=" << name
  //             << ", sourceFile=" << sourceFile << std::endl;
  // }

public:
  int numShaders;
  int numTextures;
};