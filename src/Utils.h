#pragma once

#include <glm/vec3.hpp>


namespace Utils {

enum TSeason {
    SE_Summer = 0,
    SE_Autumn,
    SE_Winter,
    SE_Spring,

    SE_Count
};

void GetNormalForTriangle(int x1, int y1, int z1,
	int x2, int y2, int z2,
	int x3, int y3, int z3,
	glm::vec3& normal);

}