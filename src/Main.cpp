#include "Terrain.h"

#include "Processor.h"
#include "Camera.h"

#include <vector>
#include <array>
#include <gl/glut.h>
#include <gl/GL.h>

CCamera curCamera;
CProcessor processor(17);

static void reshapeView(int w, int h)
{
    glViewport(0, 0, w, h);    
    curCamera.SetWH(w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();            
    gluPerspective( 40.0, static_cast<GLfloat>(w) / h, 1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        curCamera.GetPosition()[0], curCamera.GetPosition()[1], curCamera.GetPosition()[2],
        curCamera.GetTarget()[0], curCamera.GetTarget()[1], curCamera.GetTarget()[2],
        curCamera.GetUp()[0], curCamera.GetUp()[1], curCamera.GetUp()[2]);
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

static void moveInteractionByKey(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_UP:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_DOWN:
        curCamera.MoveCamera(key);
        break;
    }
}

static void moveInteractionByMouse(int x, int y)
{
    curCamera.MoveCamera(x, y); 
    glutPostRedisplay();
}

static void displayFunc() 
{
    processor.DisplayScene();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        curCamera.GetPosition()[0], curCamera.GetPosition()[1], curCamera.GetPosition()[2],
        curCamera.GetTarget()[0], curCamera.GetTarget()[1], curCamera.GetTarget()[2],
        curCamera.GetUp()[0], curCamera.GetUp()[1], curCamera.GetUp()[2]);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    curCamera.SetWH(500, 500);
    glutCreateWindow("Basic");

    glutReshapeFunc(reshapeView);
    glutDisplayFunc( displayFunc );
    glutKeyboardFunc(keyboardInteraction);
    glutSpecialFunc(moveInteractionByKey);
    glutPassiveMotionFunc(moveInteractionByMouse);
    //();

    glutMainLoop();
    return 0;
}