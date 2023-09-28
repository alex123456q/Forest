#include "Utils.h"

#include <glm/vec3.hpp> 
#include <glm/geometric.hpp> 


namespace Utils {

void GetNormalForTriangle(int x1, int y1, int z1,
	int x2, int y2, int z2,
	int x3, int y3, int z3,
	glm::vec3& normal)
{
	normal[0] = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	normal[1] = (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1);
	normal[2] = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	if (normal[0] * x1 + normal[1] * y1 + normal[2] * z1 < 0) {
		// if cos < 0 change direction
		for (int i = 0; i < 3; ++i) {
			normal[i] = -normal[i];
		}
	}
	glm::normalize(normal);
}


}