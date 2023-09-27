#include "Processor.h"

#include "Terrain.h"
#include <array>
#include <random>
#include <gl/glut.h>
#include <gl/GL.h>
#include <glm/vec3.hpp> 


typedef std::array<GLfloat, 3> vector3;

const float GreenTreeColor[3] = { 0.f, 0.2f, 0.1f };
const float WaterLevel = 0.2f;
const float MountainLevel = 0.8f;
const float GrassLevel = 0.5f;

void CProcessor::generateForest(int sizeForTerrain) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(0, sizeForTerrain - 1);

    const int treeCount = 50;
    for (int i = 0; i < treeCount; ++i) {
        treePos.push_back(std::make_pair(distrib(gen), distrib(gen)));
    }
}

CProcessor::CProcessor(int sizeForTerrain)
{
    // TODO: check sizeForTerrain 2n-1
    Utils::GenerateTerrain(sizeForTerrain, terrainArray, normalsForTerrain);
    
    generateForest(sizeForTerrain);
}


void CProcessor::DisplayScene() const
{
    displayTerrain();
    displayForest();
    glFlush();
}


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
    const std::vector<std::vector<glm::vec3>>& normalsForTerrain)
{
    glBegin(GL_TRIANGLES);

    glColor3fv(getColorForHeight(terrainArray[y1][x1]).data());

    glNormal3f(normalsForTerrain[y1][x1][0], normalsForTerrain[y1][x1][1], normalsForTerrain[y1][x1][2]);
    glVertex3i(x1, y1, round(maxHeight * terrainArray[y1][x1]));

    glNormal3f(normalsForTerrain[y2][x2][0], normalsForTerrain[y2][x2][1], normalsForTerrain[y2][x2][2]);
    glVertex3i(x2, y2, round(maxHeight * terrainArray[y2][x2]));

    glNormal3f(normalsForTerrain[y3][x3][0], normalsForTerrain[y3][x3][1], normalsForTerrain[y3][x3][2]);
    glVertex3i(x3, y3, round(maxHeight * terrainArray[y3][x3]));

    glEnd();
}

void CProcessor::displayTerrain() const
{
    //glTranslatef(0.f, 0,f, 0.f);terrainArray[0].size() / 2.f

    for (int y = 0; y < terrainArray.size() - 1; ++y) {
        for (int x = 0; x < terrainArray[0].size() - 1; ++x) {
            drawTerrainTriangle(x, y, x + 1, y, x + 1, y + 1, maxTerrainHeight, terrainArray, normalsForTerrain);
            drawTerrainTriangle(x, y, x + 1, y + 1, x, y + 1, maxTerrainHeight, terrainArray, normalsForTerrain);
        }
    }
}


static void drawTree(int x, int y, int z)
{
    glBegin(GL_TRIANGLES);

    const int treeHeight = 7;
    const int treeWidth = 4;

    glColor3fv(GreenTreeColor);
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

void CProcessor::displayForest() const
{
    for (int i = 0; i < treePos.size(); ++i) {
        const int x = treePos[i].first;
        const int y = treePos[i].second;
        if (terrainArray[y][x] < WaterLevel) {
            continue;
        }
        drawTree(x, y, round(maxTerrainHeight * terrainArray[y][x]));
    }
}
