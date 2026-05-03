#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>


class PerCamera {
public:
    glm::vec3 position;  
    glm::vec3 up;       
    glm::vec3 front;

    float yaw;
    float pitch;

    float fov;           
    float aspect;         
    float near;     
    float far;       

    PerCamera(glm::vec3 pos, glm::vec3 upVec,
              float yawInit, float pitchInit,
              float fovDeg, float aspectRatio,
              float nearVal, float farVal)
        : position(pos), up(upVec),
          yaw(yawInit), pitch(pitchInit),
          fov(fovDeg), aspect(aspectRatio),
          near(nearVal), far(farVal)
    {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position+front, up);
    }

    glm::vec3 getPosition() {
        return position;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }
    void updateCameraVectors() {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(f);
    }
};