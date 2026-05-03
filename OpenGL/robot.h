#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


struct Joint {
    glm::vec3 position;  
    glm::quat rotation;   
    float length;  
}; 

class Robot { 
    public:
    std::vector<Joint> joints;

    Robot(glm::vec3 shoulderPosition) : anchorPosition(shoulderPosition) {}

    void initSimpleArm() {
        glm::quat identity = glm::identity<glm::quat>();

        joints = {
            {{0,0,0}, identity, 12.0f},
            {{0,0,0}, identity, 12.0f},
        };
    }
        
    glm::vec3 forwardKinematics() {
        if (joints.empty()) return anchorPosition;

        glm::vec3 base = anchorPosition;
        glm::quat parentRot = glm::identity<glm::quat>();
        glm::vec3 parentPos = base;

        for (int i = 0; i < joints.size(); i++) {
            parentRot = parentRot * joints[i].rotation;
            glm::vec3 offset = parentRot * glm::vec3(0,0,joints[i].length);
            joints[i].position = parentPos + offset;
            parentPos = joints[i].position;
        }

        return parentPos;
    }

    void solveIK(int iterations = 15) {
        if (joints.empty()) return;

        for (int iter = 0; iter < iterations; iter++) {
            forwardKinematics();

            for (int i = (int)joints.size() - 1; i >= 0; i--)
            {

                glm::vec3 endEffector = forwardKinematics();
                glm::vec3 jointPos = (i == 0) ? anchorPosition : joints[i-1].position;

                glm::vec3 dirEnd = endEffector - jointPos;
                glm::vec3 dirGoal = targetPosition - jointPos;

                if (glm::length(dirEnd) < 1e-6f || glm::length(dirGoal) < 1e-6f) {
                    continue;
                }

                glm::vec3 toEnd = glm::normalize(dirEnd);
                glm::vec3 toGoal = glm::normalize(dirGoal);

                float cosAngle = glm::clamp(glm::dot(toEnd, toGoal), -1.0f, 1.0f);
                float angle = acos(cosAngle);

                glm::vec3 axis = glm::cross(toEnd, toGoal);

                if (glm::length(axis) < 1e-6f) {
                    continue;
                }

                axis = glm::normalize(axis);

                float alpha = 0.01f;

                glm::quat delta = glm::angleAxis(angle * alpha, axis);
                glm::quat targetRot = glm::normalize(delta * joints[i].rotation);

                glm::quat testRot = glm::normalize(
                    glm::slerp(joints[i].rotation, targetRot, 0.1f)
                );

                glm::quat oldRot = joints[i].rotation;
                glm::quat potentialRot = glm::normalize(glm::slerp(joints[i].rotation, targetRot, 0.1f));
                joints[i].rotation = potentialRot;

                forwardKinematics();

                bool ifInvalid = false;

                for (int k = 1; k < joints.size(); k++) {
                    if (joints[k-1].position.y < 0.0f ||
                        joints[k].position.y < 0.0f) {
                        ifInvalid = true;
                        break;
                    }
                }
                if (ifInvalid) {
                    joints[i].rotation = oldRot;
                }
                float distToTarget = glm::length(endEffector - targetPosition);

                if (distToTarget < 0.1f) {
                    break; 
                }
            }
        }
    }
    
    glm::vec3 getAnchor() {
        return anchorPosition;
    }

    void setTarget(glm::vec3 targetPoint) {
        targetPosition = targetPoint;
    }

    private:
        glm::vec3 anchorPosition;
        glm::vec3 targetPosition;

};