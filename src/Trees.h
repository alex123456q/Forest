// Trees

#pragma once

#include "IDisplayableObject.h"
#include <vector> 
#include <memory>
#include <glm/vec3.hpp>

class CTerrain;

class CTrees : public IDisplayableObject {
public:
	CTrees(int size, const std::shared_ptr<CTerrain>& _terrain);
	virtual void Display() const override;

private:
	const std::shared_ptr<CTerrain> terrain; 
	const int sizeForTerrain = 513;
	std::vector<std::pair<int, int>> treePos;

	void generateForest();
};