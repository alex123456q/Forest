#include "Terrain.h"

#include "Processor.h"
#include "Camera.h"

#include <vector>
#include <array>
#include <memory>
#include <gl/glut.h>
#include <gl/GL.h>

std::unique_ptr<CCamera> curCamera = nullptr;
std::unique_ptr<CProcessor> processor = nullptr;

static void reshapeView(int w, int h)
{
    glViewport(0, 0, w, h);    
    curCamera->SetWH(w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();            
    gluPerspective( 45.0, static_cast<GLfloat>(w) / h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        curCamera->GetPosition()[0], curCamera->GetPosition()[1], curCamera->GetPosition()[2],
        curCamera->GetTarget()[0], curCamera->GetTarget()[1], curCamera->GetTarget()[2],
        curCamera->GetUp()[0], curCamera->GetUp()[1], curCamera->GetUp()[2]);
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
        curCamera->MoveCamera(key);
        break;
    }
}

static void moveInteractionByMouse(int x, int y)
{
    curCamera->MoveCamera(x, y);
    glutPostRedisplay();
}

static void displayFunc() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int startTime = glutGet(GLUT_ELAPSED_TIME);

    processor->DisplayScene();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        curCamera->GetPosition()[0], curCamera->GetPosition()[1], curCamera->GetPosition()[2],
        curCamera->GetTarget()[0], curCamera->GetTarget()[1], curCamera->GetTarget()[2],
        curCamera->GetUp()[0], curCamera->GetUp()[1], curCamera->GetUp()[2]);

    int diffTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
    curCamera->SetTime( diffTime );
}

void init() 
{
    GLfloat light_color[] = { 1.0, 1.0, 1.0 };


    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize( 500, 500 );
    curCamera = std::unique_ptr<CCamera>( new CCamera() );
    processor = std::unique_ptr<CProcessor>( new CProcessor( 257 ) );
    curCamera->SetWH(500, 500);
    glutCreateWindow("Basic");
    init();

    glutReshapeFunc(reshapeView);
    glutDisplayFunc( displayFunc );
    glutKeyboardFunc(keyboardInteraction);
    glutSpecialFunc(moveInteractionByKey);
    glutPassiveMotionFunc(moveInteractionByMouse);
    //();

    glutMainLoop();
    return 0;
}
