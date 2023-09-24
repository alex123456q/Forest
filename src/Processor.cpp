#include "Processor.h"

#include "Terrain.h"
#include <array>
#include <gl/glut.h>
#include <gl/GL.h>

typedef std::array<GLfloat, 3> vector3;


CProcessor::CProcessor(int sizeForTerrain)
{
    // TODO: check sizeForTerrain 2n-1
    GenerateTerrain(sizeForTerrain, terrainArray);
}


void CProcessor::DisplayScene() const
{
    displayTerrain();
}


static vector3 getColorForHeight(float height)
{
    if (height > 0.8f) {
        return vector3({ height, height, height });
    }
    else if (height > 0.5f) {
        const float valRed = height - 0.3;
        const float valGreen = height - 0.5;
        return vector3({ valRed, valGreen, 0. });
    }
    else if (height > 0.2f) {
        return vector3({ 0., height, 0. });
    }
    else {
        return vector3({ 0., 0., 0.6f });
    }
}

static void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
    const std::vector<std::vector<float>>& terrainArray)
{
    glBegin(GL_TRIANGLES);
    
    const int maxHeight = 20;
    glColor3fv(getColorForHeight(terrainArray[y1][x1]).data());
    glVertex3i(x1, y1, round(maxHeight * terrainArray[y1][x1]));

    glColor3fv(getColorForHeight(terrainArray[y2][x2]).data());
    glVertex3i(x2, y2, round(maxHeight * terrainArray[y2][x2]));

    glColor3fv(getColorForHeight(terrainArray[y3][x3]).data());
    glVertex3i(x3, y3, round(maxHeight * terrainArray[y3][x3]));

    glEnd();
}

void CProcessor::displayTerrain() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    //glTranslatef(0.f, 0,f, 0.f);terrainArray[0].size() / 2.f

    for (int y = 0; y < terrainArray.size() - 1; ++y) {
        for (int x = 0; x < terrainArray[0].size() - 1; ++x) {
            drawTriangle(x, y, x + 1, y, x + 1, y + 1, terrainArray);
            drawTriangle(x, y, x + 1, y + 1, x, y + 1, terrainArray);
        }
    }

    glFlush();
}