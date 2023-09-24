#include "Terrain.h"

#include "Processor.h"

#include <vector>
#include <array>
#include <gl/glut.h>
#include <gl/GL.h>


static void reshapeView(int w, int h)
{
    glViewport(0, 0, w, h);      

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();            
   //gllOrtho(0, w, 0, h, -1, 1);  
    gluPerspective( 60.0, (GLfloat)w / h, 1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 0.0f, 50.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
}

static void keyboardInteraction(unsigned char key, int x, int y)
{
#define KEY_ESCAPE '\033'
    switch (key) {
    case KEY_ESCAPE:
        exit(0);
        break;
    }
}

static void moveInteraction(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_UP:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_DOWN:
        exit(0);
        //camera->move
        break;
    }
}

CProcessor processor(17);
static void displayFunc() 
{
    processor.DisplayScene();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Basic");

    glutDisplayFunc( displayFunc );
    glutReshapeFunc(reshapeView);
    glutKeyboardFunc(keyboardInteraction);
    glutSpecialFunc(moveInteraction);

    glutMainLoop();
    return 0;
}