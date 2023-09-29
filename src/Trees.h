// Trees

#pragma once

#include "IDisplayableObject.h"
#include "Utils.h"
#include <vector> 
#include <memory>
#include <glm/vec3.hpp>

class CTerrain;

class CTrees : public IDisplayableObject {
public:
	CTrees(int size, const std::shared_ptr<CTerrain>& _terrain);
	virtual void Display() const override;
	void ChangeSeason();

private:
	const std::shared_ptr<CTerrain> terrain; 
	const int sizeForTerrain = 513;
	std::vector<std::pair<int, int>> treePos;
	Utils::TSeason curSeason = Utils::TSeason::SE_Summer;

	void generateForest();
};