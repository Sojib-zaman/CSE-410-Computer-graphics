#include <bits/stdc++.h>
using namespace std ; 
#include <ctime>
#include <cstdlib>
#include <chrono>
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>
#include <chrono>
#include "1905067_classes.h"


point ball_position ; 
int square_length =15.0 ; 
point ball_look ; 
point ball_right ; 
point ball_up ; 
double ball_radius = 5.0 ; 
int recursion_level ;
int number_of_pixels ;
int number_of_objects ;


double angle_converter(double degree) {
    return degree * M_PI / 180.0;
}

void showPointInfo()
{
    camera_up.show("camera_up") ;
    camera_look.show("camera_look") ;
    camera_right.show("camera_right") ;
    position_of_camera.show("position_of_camera") ;
}
void keyboard(unsigned char key , int a , int b)
{
    showPointInfo() ; 
    switch (key)
    {
    case '2':
        camera_look = RodriGeneral(camera_look , camera_up , 0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , 0.1) ; 
        break;
    case '1':
        camera_look = RodriGeneral(camera_look , camera_up , -0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , -0.1) ; 
        break ; 
    case '3':
        camera_look = RodriGeneral(camera_look , camera_right , 0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_right , 0.1) ; 
        break;
    case '4':
        camera_look = RodriGeneral(camera_look , camera_right , -0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_right , -0.1) ; 

        break;
    case '6':
        camera_right = RodriGeneral(camera_right , camera_look , 0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , 0.1) ; 
        break;
    case '5':
        camera_right = RodriGeneral(camera_right , camera_look , -0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , -0.1) ; 
        break;
    default:
        break;
    }
}


//! Changes the camera position only but not the look up or right vectors
void keystrokehandler(int key , int x  , int y)
{
    showPointInfo() ;
    switch (key)
    {
    case GLUT_KEY_UP:
        position_of_camera=position_of_camera.addition(camera_look) ; 
        break;
    case GLUT_KEY_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_look) ; 
        break;
    case GLUT_KEY_LEFT:
        position_of_camera=position_of_camera.addition(camera_right) ; 
        break;
    case GLUT_KEY_RIGHT:
        position_of_camera=position_of_camera.subtraction(camera_right) ; 
        break;
    case GLUT_KEY_PAGE_UP:
        position_of_camera=position_of_camera.addition(camera_up) ; 
        break;
    case GLUT_KEY_PAGE_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_up) ; 
        break;
    default:
        printf("aeh?\n") ; 
        break;
    }
}
void init()
{
    
    glEnable(GL_DEPTH_TEST) ; 
    glMatrixMode(GL_PROJECTION) ; 
    glLoadIdentity();
    gluPerspective(70,1,1,1000); 
    
    position_of_camera = point(202.405,210.518,100.642) ; 
    camera_look = point(-0.465755,-0.547041,-0.695571) ;
    camera_right = point(0.781163,-0.623468,-0.0327333) ; 
    camera_up = point(-0.41576,-0.5586,0.717711) ; 

    ball_position = point(150,150,5) ; 
    ball_look = point(1,0,0) ; 
    ball_right = point(0,1,0) ; 
    ball_up = point(0,0,1) ;
    

}

void drawFloor()
{
	
    double x=-100.0,y=-100.0;
    for(int i=0 ; i<30 ; i++)
    {
        x=i*square_length ; 
        for(int j=0 ; j<30 ; j++)
        {
            y=j*square_length ; 

            if((i+j)%2==1)setcolor(black) ; 
            else setcolor(white) ; 

            glBegin(GL_QUADS) ; 
            {
                glVertex2f(x,y) ;
                glVertex2f(x+square_length,y) ;
                glVertex2f(x+square_length,y+square_length) ;
                glVertex2f(x,y+square_length) ;

            }
            
            glEnd() ; 
        }
        
    }

	
}


// display function 
void RayTracing()
{


    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); // Now working on model view matrix 
    glLoadIdentity(); // all previous matrix cleared 

    gluLookAt(position_of_camera.getx() , position_of_camera.gety() , position_of_camera.getz() , 
    position_of_camera.getx()+camera_look.getx() , position_of_camera.gety()+camera_look.gety() , position_of_camera.getz()+camera_look.getz() , 
    camera_up.getx() , camera_up.gety() , camera_up.getz()
    ) ; 

    glPushMatrix() ; 
    drawFloor() ; 
    glPopMatrix() ;
   

    glutSwapBuffers() ; 
}
void idle()
{    
    glutPostRedisplay(); 
}
void timer(int val)
{

}
void loaddata()
{
    ifstream fin ;
    fin.open("scene.txt") ;

    fin>>recursion_level ;
    fin>>number_of_pixels ;
    fin>>number_of_objects ;
    cout<<recursion_level<<" "<<number_of_pixels<<" "<<number_of_objects<<endl ;
}  
        

 
int main(int argc , char** argv)
{
    loaddata() ; 
    glutInit(&argc , argv) ; 
    glutInitWindowPosition(1000,200) ; 
    glutInitWindowSize(1000,1000) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; //RGB - Double buffer - depth buffer 
    glutCreateWindow("Ray tracing") ; 
    
    init() ; 
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard); 
    glutDisplayFunc(RayTracing) ;
    glutTimerFunc(50,timer,0);
    glutIdleFunc(idle) ;
    glutMainLoop() ; 
    return 0 ; 
}
