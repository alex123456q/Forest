#pragma once

#include <vector>

class CProcessor {
public:
	CProcessor(int sizeForTerrain=17);

	void DisplayScene() const;
private:
	std::vector<std::vector<float>> terrainArray;

	void displayTerrain() const;
};