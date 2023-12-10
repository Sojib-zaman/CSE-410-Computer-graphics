#include <bits/stdc++.h>
using namespace std ; 

#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>

void init()
{
    glEnable(GL_DEPTH_TEST) ; 
    glMatrixMode(GL_PROJECTION) ; 
    glLoadIdentity();
    gluPerspective(80,1,1,1000); 
}
void drawTriangle(int length)
{
    glBegin(GL_TRIANGLES) ; 
    glColor3f(1,0,0) ; 
    glVertex3f(0,0,1) ; 
    glVertex3f(0,1,0) ;    
    glVertex3f(1,0,0) ;
    glEnd() ;  
}
void magic_cube()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10,10,10,5,5,5,0,1,0) ;
    glMatrixMode(GL_MODELVIEW);

    drawTriangle(2) ; 
    glutSwapBuffers() ; 
}
void idle()
{    
    glutPostRedisplay(); 
}
int main(int argc , char** argv)
{
    glutInit(&argc , argv) ; 
    glutInitWindowPosition(1000,200) ; 
    glutInitWindowSize(600,600) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; //RGB - Double buffer - depth buffer 
    glutCreateWindow("Magic Cube") ; 

    init() ; 
    glutDisplayFunc(magic_cube) ;
    glutIdleFunc(idle) ;
    glutMainLoop() ; 
    return 0 ; 
}