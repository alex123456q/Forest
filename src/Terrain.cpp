// Terrain

#include "Terrain.h"

#include "Utils.h"
#include <cmath>
#include <cassert>
#include <random>
#include <array>
#include <algorithm>
#include <glm/vec3.hpp> 
#include <glm/geometric.hpp> 
#include <gl/glut.h>
#include <gl/GL.h>
//TODO: enable logging

typedef std::array<GLfloat, 3> vector3;

const float WaterLevel = 0.2f;
const float MountainLevel = 0.8f;
const float GrassLevel = 0.5f;

static vector3 getColorForHeight(float height)
{
	if (height > MountainLevel) {
		return vector3({ height, height, height });
	}
	else if (height > GrassLevel) {
		const float valRed = height - 0.3;
		const float valGreen = height - 0.5;
		return vector3({ valRed, valGreen, 0. });
	}
	else if (height > WaterLevel) {
		return vector3({ 0., height, 0. });
	}
	else {
		return vector3({ 0., 0., 0.6f });
	}
}

static void drawTerrainTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int maxHeight,
	const std::vector<std::vector<float>>& terrainArray,
	const std::vector<std::vector<glm::vec3>>& normals)
{
	glBegin(GL_TRIANGLES);

	glColor3fv(getColorForHeight(terrainArray[y1][x1]).data());

	glNormal3f(normals[y1][x1][0], normals[y1][x1][1], normals[y1][x1][2]);
	glVertex3i(x1, y1, round(maxHeight * terrainArray[y1][x1]));

	glNormal3f(normals[y2][x2][0], normals[y2][x2][1], normals[y2][x2][2]);
	glVertex3i(x2, y2, round(maxHeight * terrainArray[y2][x2]));

	glNormal3f(normals[y3][x3][0], normals[y3][x3][1], normals[y3][x3][2]);
	glVertex3i(x3, y3, round(maxHeight * terrainArray[y3][x3]));

	glEnd();
}


void CTerrain::Display() const
{
	for (int y = 0; y < terrainArray.size() - 1; ++y) {
		for (int x = 0; x < terrainArray[0].size() - 1; ++x) {
			drawTerrainTriangle(x, y, x + 1, y, x + 1, y + 1, MaxTerrainHeight, terrainArray, normals);
			drawTerrainTriangle(x, y, x + 1, y + 1, x, y + 1, MaxTerrainHeight, terrainArray, normals);
		}
	}
}

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

float CTerrain::GetWaterLevel() 
{
	return WaterLevel * MaxTerrainHeight;
}

 void CTerrain::normalizeTerrain()
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

void CTerrain::zeroLowestValues()
{
	// we want something like rivers or lakes on one level
	const float lowestValue = 0.2f;
	for (int i = 0; i < terrainArray.size(); ++i) {
		for (int j = 0; j < terrainArray[i].size(); ++j) {
			terrainArray[i][j] = terrainArray[i][j] < lowestValue ? 0.f : terrainArray[i][j];
		}
	}
}


void CTerrain::getNormalsForArray()
{
	std::vector<std::vector<glm::vec3>> normalsForTerrainSquares;
	normalsForTerrainSquares.resize(terrainArray.size() - 1);
	for (int y = 0; y < terrainArray.size() - 1; ++y) {
		normalsForTerrainSquares[y].resize(terrainArray[0].size() - 1);
		for (int x = 0; x < terrainArray[0].size() - 1; ++x) {
			// lower triangle
			Utils::GetNormalForTriangle(x, y, terrainArray[y][x],
				x + 1, y + 1, terrainArray[y][x],
				x, y + 1, terrainArray[y][x],
				normalsForTerrainSquares[y][x]);
		}
	}
	normals.resize(terrainArray.size());
	for (int y = 0; y < terrainArray.size(); ++y) {
		normals[y].resize(terrainArray[0].size());
	}

	normals[0][0] = normalsForTerrainSquares[0][0];
	//memcpy(normalsForTerrainSquares[0][0], normals[0][0], 3 * sizeof(float));
	for (int y = 1; y < terrainArray.size() - 2; ++y) {
		normals[y][0] = (normalsForTerrainSquares[y][0] + normalsForTerrainSquares[y + 1][0]) * 0.5f;
		const int lastColumn = terrainArray[0].size() - 1;
		normals[y][lastColumn] = (normalsForTerrainSquares[y][lastColumn - 1] + normalsForTerrainSquares[y + 1][lastColumn - 1]) * 0.5f;
	}
	for (int x = 1; x < terrainArray[0].size() - 2; ++x) {
		normals[0][x] = (normalsForTerrainSquares[0][x] + normalsForTerrainSquares[0][x + 1]) * 0.5f;
		const int lastRow = terrainArray.size() - 1;
		normals[lastRow][x] =
			(normalsForTerrainSquares[lastRow - 1][x] + normalsForTerrainSquares[lastRow - 1][x + 1]) * 0.5f;
	}
	for (int y = 1; y < terrainArray.size() - 2; ++y) {
		for (int x = 1; x < terrainArray[0].size() - 2; ++x) {

			normals[y][x] = (
				normalsForTerrainSquares[y - 1][x - 1] +
				normalsForTerrainSquares[y][x - 1] +
				normalsForTerrainSquares[y][x] +
				normalsForTerrainSquares[y - 1][x]
				) * 0.25f;
		}
	}
}

void CTerrain::generateTerrain()
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

	normalizeTerrain();
	zeroLowestValues();

	getNormalsForArray();
}
