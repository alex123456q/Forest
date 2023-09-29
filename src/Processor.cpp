#include "Processor.h"

#include "Terrain.h"
#include "Trees.h"
#include <array>
#include <random>
#include <gl/glut.h>
#include <gl/GL.h>
#include <glm/vec3.hpp> 


CProcessor::CProcessor(int sizeForTerrain) : terrain( new CTerrain( sizeForTerrain ) ),
trees( new CTrees( sizeForTerrain, terrain ) )
{
}


void CProcessor::DisplayScene() const
{
    terrain->Display();
    trees->Display();
    glFlush();
}

void CProcessor::ChangeSeason() const 
{
    trees->ChangeSeason();
}