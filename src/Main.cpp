#include "Terrain.h"
#include <vector>
#include <gl/glut.h>
#include <gl/GL.h>


const GLfloat Red[3] = { 1.0, 0.0, 0.0 };
const GLfloat Blue[3] = { 0.0, 0.0, 1.0 };
const GLfloat Green[3] = { 0.0, 1.0, 0.0 };


void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, 
    std::vector<std::vector<int>>& terrainArray, int /*maxHeight*/)
{
    // TODO: paint by height
    glBegin(GL_TRIANGLES);
    glColor3fv(Red);
    glVertex3i(x1, y1, terrainArray[y1][x1]);
    glColor3fv(Green);
    glVertex3i(x2, y2, terrainArray[y2][x2]);
    glColor3fv(Blue);
    glVertex3i(x3, y3, terrainArray[y3][x3]);
    glEnd();
}

void displayTriangles(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glTranslatef(-8, -8, -8.0);


    const int sizeForTerrain = 17;//513;//1024 + 1;
    const int maxHeight = 100;
    // TODO: float
    // TODO: generate in another place
    std::vector<std::vector<int>> terrainArray;
    GenerateTerrain( sizeForTerrain, maxHeight, terrainArray );

    for (int y = 0; y < sizeForTerrain - 1; ++y) {
        for (int x = 0; x < sizeForTerrain - 1; ++x) {
            drawTriangle( x, y, x + 1, y, x + 1, y + 1, terrainArray, maxHeight);
            drawTriangle( x, y, x + 1, y + 1, x, y + 1, terrainArray, maxHeight);
        }
    }

    glFlush();
}

void reshapeView(int w, int h)
{
    glViewport(0, 0, w, h);      

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();            
   //gllOrtho(0, w, 0, h, -1, 1);  
    gluPerspective( 40.0, (GLfloat)w / h, 1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        0.0f, 0.0f, 50.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Basic");

    glutDisplayFunc(displayTriangles);
    glutReshapeFunc(reshapeView);

    glutMainLoop();
    return 0;
}