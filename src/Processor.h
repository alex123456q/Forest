#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp> 

class CTrees;
class CTerrain;

class CProcessor {
public:
	CProcessor(int sizeForTerrain=17);

	void DisplayScene() const;
	void ChangeSeason() const;
private:
	const std::shared_ptr<CTerrain> terrain;
	const std::shared_ptr<CTrees> trees;

	const int maxTerrainHeight = 100;
};