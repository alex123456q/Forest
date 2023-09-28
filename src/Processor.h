#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp> 

class CTrees;
class CTerrain;

class CProcessor {
public:
	CProcessor(int sizeForTerrain=17);

	//void Run(int argc, char** argv) const;
	void DisplayScene() const;
private:
	const std::shared_ptr<CTerrain> terrain;
	const std::shared_ptr<CTrees> trees;

	const int maxTerrainHeight = 100;

	void displayForest() const;

	void generateForest(int sizeForTerrain);
};