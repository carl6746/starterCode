#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/string_cast.hpp>


class ISceneLoader {
    public:
    virtual ~ISceneLoader() = default;

    virtual void reserveCameras(size_t count) = 0;
    virtual void reserveLights(size_t count) = 0;
    virtual void reserveShapes(size_t count) = 0;
    virtual void reserveShaders(size_t count) = 0;

    struct vec {
        double x, y, z;
    };

    struct SceneParams {
        vec bgColor = {0.0,0.0,0.0};
        std::string envMapFile = "";
    };

    virtual void setSceneParams(const SceneParams &sparams) = 0;

    struct ShaderProperty {
        vec data = {0.0, 0.0, 0.0}; // Default to black
        std::string textureName = "";
        bool hasTexture = false;
    };

    struct ShaderDesc{
        std::string name;
        std::string type;
        ShaderProperty diffuse;
        ShaderProperty specular;
        ShaderProperty normal;
        ShaderProperty emission;
        double phongExp = 1.0;
        double mirrorCoef = 0.0;
        double roughness = 0.0;
        vec attenuationCoef = {0.0,0.0,0.0};
        double refractiveIndex = 1.0;
    };

    virtual void addCamera(const std::string &name, const std::string &type,
                         const vec &pos, const vec &viewDir, double focalLength,
                         double imagePlaneWidth) = 0;
    virtual void addPointLight(const vec &pos, const vec &intensity) = 0;
    virtual void addAreaLight(const ISceneLoader::vec &pos,
                        const ISceneLoader::vec &color,
                        const ISceneLoader::vec &normal, double width,
                        double length) = 0;
    virtual void addShader(const ISceneLoader::ShaderDesc &shaderDesc) = 0;

    struct ShapeDesc {
        std::string type;
        std::string name;

        bool hasXForm = false;
        glm::mat4 localXForm;

        std::string shaderNameReference;

        vec center;   // for sphere
        double radius; // for sphere

        vec v0, v1, v2; // triangle vertices

        vec minPt, maxPt; // box minPt, maxPt

        std::string meshFilePath;

        std::string instanceId;
    };

    virtual void addShape(const ShapeDesc &desc)=0;
};