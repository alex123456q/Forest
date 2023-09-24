#pragma once

#include <array>
#include <glm/vec3.hpp> 


class CCamera {
public:
    void SetPosition(float x, float y, float z);
    const glm::vec3& GetPosition() const;
    const glm::vec3 GetTarget() const;
    void SetUp(float x, float y, float z);
    const glm::vec3& GetUp() const;

    void MoveCamera(int key);
    void MoveCamera(int x, int y);
    void SetWH(int w, int h);
private:
    glm::vec3 position{ 40., 40., 20. };
    glm::vec3 direction{ -1., 0., 0. };
    glm::vec3 up{ 0., 0., 1. };
    float alpha = 0.f; // 0..2pi along X horizontalAngle
    float beta = -3.14f; // -pi .. pi along Y verticalAngle
    int width = 1024;
    int height = 1024;    

    const float stepSize = 1.f;
};