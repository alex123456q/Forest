#pragma once

#include <vector>
#include <glm/vec3.hpp> 

class CProcessor {
public:
	CProcessor(int sizeForTerrain=17);

	//void Run(int argc, char** argv) const;
	void DisplayScene() const;
private:
	std::vector<std::vector<float>> terrainArray;
	std::vector<std::vector<glm::vec3>> normalsForTerrain;
	std::vector<std::pair<int,int>> treePos;

	const int maxTerrainHeight = 100;

	void displayTerrain() const;
	void displayForest() const;

	void generateForest(int sizeForTerrain);
};