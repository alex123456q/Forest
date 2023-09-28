// Terrain

#pragma once

#include "IDisplayableObject.h"
#include <vector> 
#include <glm/vec3.hpp>

class CTerrain : public IDisplayableObject {
public:
	CTerrain(int size) : sizeForTerrain(size) { generateTerrain(); };
	virtual void Display() const override;
	
	float GetHeightInPoint(int x, int y) const { return terrainArray[y][x] * MaxTerrainHeight; };
	float GetWaterLevel();
private:
	const int sizeForTerrain = 513;
	const int MaxTerrainHeight = 100;
	std::vector<std::vector<float>> terrainArray;
	std::vector<std::vector<glm::vec3>> normals;

	void normalizeTerrain();
	void zeroLowestValues();
	void getNormalsForArray();
	void generateTerrain();
};