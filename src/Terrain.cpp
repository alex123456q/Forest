#include "Terrain.h"

#include <cmath>
#include <cassert>
#include <random>

//TODO: enable logging

static void genTerrainInsideBlock(std::vector<std::vector<int>>& terrainArray,
	int left, int top, int right, int bottom, std::mt19937& gen)
{
	assert(right - left == bottom - top);
	assert(left >= 0 && top >= 0 &&
		bottom < terrainArray.size() && right < terrainArray[0].size());
	if (bottom - top <= 1) {
		return;
	}
	// 0 0 1 0 // 0 1 1 1 // 1 1 1 0 // 1 0 0 0 // all
	const float addingCoeff = 0.3f;
	std::uniform_int_distribution<> distrib(0, round((right - left) * addingCoeff));

	const int heightMiddle = (top + bottom) / 2; 
	const int widthMiddle = (left + right) / 2;
	int adding = distrib(gen);
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

static void zeroLowestValues(std::vector<std::vector<int>>& terrainArray)
{
	// we want something like rivers or lakes on one level
	const int lowestValue = 5;
	for (int i = 0; i < terrainArray.size(); ++i) {
		for (int j = 0; j < terrainArray[i].size(); ++j) {
			terrainArray[i][j] = terrainArray[i][j] < lowestValue ? lowestValue : terrainArray[i][j];
		}
	}
}

void GenerateTerrain( int sizeForTerrain, int maxHeight,
	std::vector<std::vector<int>>& terrainArray )
{
	terrainArray.clear();
	terrainArray.resize( sizeForTerrain );
	for (int i = 0; i < terrainArray.size(); ++i) {
		terrainArray[i].resize( sizeForTerrain );
		std::fill(terrainArray[i].begin(), terrainArray[i].end(), 0);

	}

	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, maxHeight);
	// init
	terrainArray[0][0] = distrib(gen);
	terrainArray[0][sizeForTerrain - 1] = distrib(gen);
	terrainArray[sizeForTerrain - 1][0] = distrib(gen);
	terrainArray[sizeForTerrain - 1][sizeForTerrain - 1] = distrib(gen);

	genTerrainInsideBlock( terrainArray, 0, 0, sizeForTerrain - 1, sizeForTerrain - 1, gen );

	zeroLowestValues( terrainArray );
}