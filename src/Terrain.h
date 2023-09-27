#pragma once

#include <vector> 
#include <glm/vec3.hpp>


namespace Utils {

void GenerateTerrain(int sizeForTerrain, std::vector<std::vector<float>>& terrainArray,
	std::vector<std::vector<glm::vec3>>& normals);

void GetNormalForTriangle(int x1, int y1, int z1,
	int x2, int y2, int z2,
	int x3, int y3, int z3,
	glm::vec3& normal);

}