#include "Terrain.h"

#include <cmath>
#include <cassert>
#include <random>
#include <algorithm>
#include <glm/vec3.hpp> 
#include <glm/geometric.hpp> 

//TODO: enable logging

namespace Utils {

static void genTerrainInsideBlock(std::vector<std::vector<float>>& terrainArray,
	int left, int top, int right, int bottom, std::mt19937& gen)
{
	assert(right - left == bottom - top && left < right);
	assert(left >= 0 && top >= 0 &&
		bottom < terrainArray.size() && right < terrainArray[0].size());
	if (bottom - top <= 1) {
		return;
	}
	// 0 0 1 0 // 0 1 1 1 // 1 1 1 0 // 1 0 0 0 // all
	const float addingCoeff = 0.1f;
	const float halfOfMaxValueAdding = (right - left) * addingCoeff / 2.f;
	std::uniform_real_distribution<float> distrib(-halfOfMaxValueAdding, halfOfMaxValueAdding);

	const int heightMiddle = (top + bottom) / 2;
	const int widthMiddle = (left + right) / 2;
	float adding = distrib(gen);
	terrainArray[heightMiddle][left]
		= (terrainArray[top][left] + terrainArray[bottom][left]) / 2 + adding;
	adding = distrib(gen);
	terrainArray[heightMiddle][right]
		= (terrainArray[top][right] + terrainArray[bottom][right]) / 2 + adding;
	adding = distrib(gen);
	terrainArray[top][widthMiddle]
		= (terrainArray[top][left] + terrainArray[top][right]) / 2 + adding;
	adding = distrib(gen);
	terrainArray[bottom][widthMiddle]
		= (terrainArray[bottom][left] + terrainArray[bottom][right]) / 2 + adding;
	adding = distrib(gen);
	terrainArray[heightMiddle][widthMiddle]
		= (terrainArray[top][left] + terrainArray[bottom][left] +
			terrainArray[top][right] + terrainArray[bottom][right]) / 4 + adding;

	genTerrainInsideBlock(terrainArray, left, top, widthMiddle, heightMiddle, gen);
	genTerrainInsideBlock(terrainArray, widthMiddle, top, right, heightMiddle, gen);
	genTerrainInsideBlock(terrainArray, left, heightMiddle, widthMiddle, bottom, gen);
	genTerrainInsideBlock(terrainArray, widthMiddle, heightMiddle, right, bottom, gen);
}

static void normalizeTerrain(std::vector<std::vector<float>>& terrainArray)
{
	// we want something like rivers or lakes on one level
	float minVal = 10000;
	float maxVal = 0;
	for (int i = 0; i < terrainArray.size(); ++i) {
		for (int j = 0; j < terrainArray[i].size(); ++j) {
			minVal = std::min(minVal, terrainArray[i][j]);
			maxVal = std::max(maxVal, terrainArray[i][j]);
		}
	}
	if (maxVal - minVal < 1e-6) {
		return;
	}
	for (int i = 0; i < terrainArray.size(); ++i) {
		for (int j = 0; j < terrainArray[i].size(); ++j) {
			terrainArray[i][j] = (terrainArray[i][j] - minVal) / (maxVal - minVal);
		}
	}
}

static void zeroLowestValues(std::vector<std::vector<float>>& terrainArray)
{
	// we want something like rivers or lakes on one level
	const float lowestValue = 0.2f;
	for (int i = 0; i < terrainArray.size(); ++i) {
		for (int j = 0; j < terrainArray[i].size(); ++j) {
			terrainArray[i][j] = terrainArray[i][j] < lowestValue ? 0.f : terrainArray[i][j];
		}
	}
}

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


static void getNormalsForArray( std::vector<std::vector<float>>& terrainArray,
	std::vector<std::vector<glm::vec3>>& normalsForTerrain )
{
	std::vector<std::vector<glm::vec3>> normalsForTerrainSquares;
	normalsForTerrainSquares.resize(terrainArray.size() - 1);
	for (int y = 0; y < terrainArray.size() - 1; ++y) {
		normalsForTerrainSquares[y].resize(terrainArray[0].size() - 1);
		for (int x = 0; x < terrainArray[0].size() - 1; ++x) {
			// lower triangle
			GetNormalForTriangle(x, y, terrainArray[y][x],
				x + 1, y + 1, terrainArray[y][x],
				x, y + 1, terrainArray[y][x],
				normalsForTerrainSquares[y][x]);
		}
	}
	normalsForTerrain.resize(terrainArray.size());
	for (int y = 0; y < terrainArray.size(); ++y) {
		normalsForTerrain[y].resize(terrainArray[0].size());
	}

	normalsForTerrain[0][0] = normalsForTerrainSquares[0][0];
	//memcpy(normalsForTerrainSquares[0][0], normalsForTerrain[0][0], 3 * sizeof(float));
	for (int y = 1; y < terrainArray.size() - 2; ++y) {
		normalsForTerrain[y][0] = (normalsForTerrainSquares[y][0] + normalsForTerrainSquares[y + 1][0]) * 0.5f;
		const int lastColumn = terrainArray[0].size() - 1;
		normalsForTerrain[y][lastColumn] = (normalsForTerrainSquares[y][lastColumn - 1] + normalsForTerrainSquares[y + 1][lastColumn - 1]) * 0.5f;
	}
	for (int x = 1; x < terrainArray[0].size() - 2; ++x) {
		normalsForTerrain[0][x] = (normalsForTerrainSquares[0][x] + normalsForTerrainSquares[0][x + 1]) * 0.5f;
		const int lastRow = terrainArray.size() - 1;
		normalsForTerrain[lastRow][x] =
			(normalsForTerrainSquares[lastRow - 1][x] + normalsForTerrainSquares[lastRow - 1][x + 1]) * 0.5f;
	}
	for (int y = 1; y < terrainArray.size() - 2; ++y) {
		for (int x = 1; x < terrainArray[0].size() - 2; ++x) {

			normalsForTerrain[y][x] = (
				normalsForTerrainSquares[y - 1][x - 1] +
				normalsForTerrainSquares[y][x - 1] +
				normalsForTerrainSquares[y][x] +
				normalsForTerrainSquares[y - 1][x]
				) * 0.25f;
		}
	}
}

void GenerateTerrain(int sizeForTerrain, std::vector<std::vector<float>>& terrainArray,
	std::vector<std::vector<glm::vec3>>& normalsForTerrain)
{
	terrainArray.clear();
	terrainArray.resize(sizeForTerrain);
	for (int i = 0; i < terrainArray.size(); ++i) {
		terrainArray[i].resize(sizeForTerrain);
		std::fill(terrainArray[i].begin(), terrainArray[i].end(), 0.f);

	}

	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<float> distrib(0.f, 1.f);
	// init
	terrainArray[0][0] = distrib(gen);
	terrainArray[0][sizeForTerrain - 1] = distrib(gen);
	terrainArray[sizeForTerrain - 1][0] = distrib(gen);
	terrainArray[sizeForTerrain - 1][sizeForTerrain - 1] = distrib(gen);

	genTerrainInsideBlock(terrainArray, 0, 0, sizeForTerrain - 1, sizeForTerrain - 1, gen);

	normalizeTerrain(terrainArray);
	zeroLowestValues(terrainArray);

	getNormalsForArray(terrainArray, normalsForTerrain);
}

}