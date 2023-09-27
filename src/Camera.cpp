#include "Camera.h"

#include <cmath>
#include <cassert>
#include <gl/glut.h>
#include <glm/geometric.hpp> 

#include <iostream>

void CCamera::SetPosition(float x, float y, float z)
{
	position = { x, y, z };
}

const glm::vec3& CCamera::GetPosition() const 
{
	return position;
}

const glm::vec3 CCamera::GetTarget() const
{
	return position + direction;
}

void CCamera::SetUp(float x, float y, float z)
{
	up = { x, y, z };
}

const glm::vec3& CCamera::GetUp() const
{
	return up;
}

void CCamera::SetWH(int w, int h)
{
    width = w;
    height = h;
}

void CCamera::SetTime(float time)
{
    deltaTime = time;
}

void CCamera::MoveCamera(int x, int y)
{
    glutWarpPointer( width / 2, height / 2);

    const float speed = deltaTime * 0.00001f;
    alpha += speed * (width / 2 - x);
    beta += speed * (height / 2 - y);

    direction[0] = cos(alpha) * cos(beta);
    direction[1] = sin(alpha) * cos(beta);
    direction[2] = sin(beta);

    direction = glm::normalize(direction);
    glm::vec3 right = glm::cross({ 0.f, 0.f, 1.f }, direction);
    glm::vec3 up = glm::cross(right, direction);
}

void CCamera::MoveCamera(int key)
{
    const float speed = deltaTime * 0.01f;

    switch (key) {
        case GLUT_KEY_UP:
            position += direction * stepSize * speed;
            break;

        case GLUT_KEY_DOWN:
            position -= direction * stepSize * speed;
            break;

        case GLUT_KEY_LEFT:
            glm::vec3 leftDirection = glm::cross(up, direction);
            leftDirection = glm::normalize(leftDirection);
            position += leftDirection * stepSize * speed;
            break;
        case GLUT_KEY_RIGHT:
            glm::vec3 rightDirection = glm::cross(direction, up); 
            rightDirection = glm::normalize(rightDirection);
            position += rightDirection * stepSize * speed;
            break;
        default:
            break;
    }
    std::cout << "pos" << position[0] << " " << position[1] << " " << position[2] << std::endl; 
    std::cout << "direction" << direction[0] << " " << direction[1] << " " << direction[2] << std::endl; 

}
