
#include <gl/glut.h>
#include <gl/GL.h>

const GLfloat Red[3] = { 1.0, 0.0, 0.0 };
const GLfloat Blue[3] = { 0.0, 0.0, 1.0 };
const GLfloat Green[3] = { 0.0, 1.0, 0.0 };

void displayTriangle(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3fv(Red); 
    glVertex2i(10, 10);
    glColor3fv(Green); 
    glVertex2i(100, 100);
    glColor3fv(Blue); 
    glVertex2i(10, 200);
    glEnd();
    glFlush();
}

void reshapeView(int w, int h)
{
    glViewport(0, 0, w, h);      

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();            
    glOrtho(0, w, 0, h, -1, 1);  

    //glScalef(1, -1, 1);          
    //glTranslatef(0, -h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glutCreateWindow("Basic");

    glutDisplayFunc(displayTriangle);
    glutReshapeFunc(reshapeView);

    glutMainLoop();
    return 0;
}