// Trees

#include "Trees.h"
#include "Utils.h"
#include "Terrain.h"

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
const vector3 GreenTreeColor = { 0.f, 0.2f, 0.1f };
const vector3 YellowTreeColor = { 0.8f, 0.5f, 0.3f };
const vector3 WhiteTreeColor = { 0.7f, 0.7f, 0.8f };
const vector3 YoungGreenTreeColor = { 0.6f, 0.9f, 0.6f };

static_assert( Utils::TSeason::SE_Count == 4, "Seasons count changed. Check code below.");
const std::array<vector3, 4> SeasonColors = { GreenTreeColor, YellowTreeColor, WhiteTreeColor, YoungGreenTreeColor };

CTrees::CTrees(int size, const std::shared_ptr<CTerrain>& _terrain) 
    : sizeForTerrain(size), terrain(_terrain) 
{
    generateForest();
};

static void drawTree(int x, int y, int z, const vector3& color)
{
    glBegin(GL_TRIANGLES);

    const int treeHeight = 7;
    const int treeWidth = 4;

    glColor3fv( color.data() );
    glm::vec3 normal;
    Utils::GetNormalForTriangle(x - treeWidth / 2, y, z,
        x, y, z + treeHeight,
        x + treeWidth / 2, y, z,
        normal);
    glNormal3f(normal[0], normal[1], normal[2]);
    glVertex3i(x - treeWidth / 2, y, z);
    glVertex3i(x, y, z + treeHeight);
    glVertex3i(x + treeWidth / 2, y, z);

    Utils::GetNormalForTriangle(x, y - treeWidth / 2, z,
        x, y, z + treeHeight,
        x, y + treeWidth / 2, z,
        normal);
    glNormal3f(normal[0], normal[1], normal[2]);
    glVertex3i(x, y - treeWidth / 2, z);
    glVertex3i(x, y, z + treeHeight);
    glVertex3i(x, y + treeWidth / 2, z);

    glEnd();
}


void CTrees::Display() const
{
    for (int i = 0; i < treePos.size(); ++i) {
        const int x = treePos[i].first;
        const int y = treePos[i].second;
        if ( terrain->GetHeightInPoint(x, y) < terrain->GetWaterLevel() ) {
            continue;
        }
        drawTree(x, y, round(terrain->GetHeightInPoint(x, y)), SeasonColors[curSeason]);
    }
}

void CTrees::ChangeSeason() 
{
    curSeason = static_cast<Utils::TSeason>( (curSeason + 1) % Utils::TSeason::SE_Count );
}

void CTrees::generateForest()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, sizeForTerrain - 1);

    const int treeCount = 50;
    for (int i = 0; i < treeCount; ++i) {
        treePos.push_back(std::make_pair(distrib(gen), distrib(gen)));
    }
}