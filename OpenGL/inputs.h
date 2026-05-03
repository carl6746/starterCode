#pragma once
#include "robot.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

class Robot;
class Camera;
struct GLFWwindow;

class Input {
    public:
        static void processKeyboard(GLFWwindow* window, Robot& robot, PerCamera& camera, float dt, float forward, float right, float rotX, float rotY, float rotZ, float scale) {
            float moveRatePerFrame = 5.0f*dt;
            float rotSpeed = 0.002f;
            float scaleSpeed = 0.001f;
            float speed = 1.5f;


            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
                robot.joints[1].rotation *= glm::angleAxis(speed * dt,
                                                        glm::vec3(1,0,0));
            }
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
                robot.joints[2].rotation *= glm::angleAxis(speed * dt,
                                                        glm::vec3(0,0,1));
            }
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera.position += forward * moveRatePerFrame;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera.position -= forward * moveRatePerFrame;
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera.position -= right * moveRatePerFrame;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera.position += right * moveRatePerFrame;
            }
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                    rotX -= rotSpeed;
                } else {
                    rotX += rotSpeed;           
                }
            }
            if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
                if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                    rotY -= rotSpeed;
                } else {
                    rotY += rotSpeed;           
                }
            }
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
                if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                    rotZ -= rotSpeed;
                } else {
                    rotZ += rotSpeed;           
                }
            }
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            std::cout << "fps: " << 1.0 / dt << std::endl;
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                scale += scaleSpeed;  
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                scale -= scaleSpeed;   
            }
            static bool mWasDown = false;
            if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
                if (!mWasDown) {
                    mouseMode = !mouseMode;

                    if (mouseMode) {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        firstMouse = true; // prevents camera jump
                    } else {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                }
                mWasDown = true;
            } else {
                mWasDown = false;
            }
            
        };
        static void processMouseMode(GLFWwindow*) {
            
        };
    private:
        static inline bool mouseMode = true;
        static inline bool firstMouse = true;
        static inline float lastX = 0.0f;
        static inline float lastY = 0.0f;
        static inline bool rightMouseWasDown = false;
};