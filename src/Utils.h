#pragma once

#include <glm/vec3.hpp>


namespace Utils {

void GetNormalForTriangle(int x1, int y1, int z1,
	int x2, int y2, int z2,
	int x3, int y3, int z3,
	glm::vec3& normal);

}