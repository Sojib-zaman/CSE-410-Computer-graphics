#include <bits/stdc++.h>
using namespace std ; 

#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>
#include "helper.h"

// 4 global parameters , one for the position 
// 3 unit vectors for up , look and right 



void draw_axes()
{
   glBegin(GL_LINES);
   {
    setcolor(magenta);
    glVertex3f(-400,0,0) ; 
    glVertex3f(400,0,0) ; 

    setcolor(yellow) ; 
    glVertex3f(0,-400,0) ; 
    glVertex3f(0,400,0) ; 

    setcolor(green);
    glVertex3f(0,0,-400) ; 
    glVertex3f(0,0,400) ; 

   }glEnd() ;
}

void init()
{
    


    // position_of_camera=point(10.0 , 10.0 , 5.0) ; 

    // camera_up=point(0,0,1.0) ; 
    // camera_right=point(-1.0/sqrt(2.0) , 1.0/sqrt(2.0) , 0.0 );
    // camera_look=point(-1.0/sqrt(2.0) , -1.0/sqrt(2.0) , 0.0);
    
    // camera_up=point(3.0/sqrt(18.0) ,0 , -3.0/sqrt(18.0) ) ; 
    // camera_right=point(-1.0/sqrt(6.0) , 2.0/sqrt(6.0) , -1.0/sqrt(6.0) );
    // camera_look=point(-1.0/sqrt(3.0) , -1.0/sqrt(3.0) , -1.0/sqrt(3.0));

    position_of_camera=point(8.9513,7.19919,7.14119) ; 

    camera_up=point(0.818987,-0.409701,-0.401753) ; 
    camera_right=point(0.00718421,0.707414,-0.706763 );
    camera_look=point(-0.573767,-0.575943,-0.582305);
    
    glMatrixMode(GL_PROJECTION) ; 
    glLoadIdentity();
    //. Field of view , aspect ratio , znear , zfar  
    gluPerspective(80,1,1,1000); 


}

void drawSquare(double a)
{
     glBegin(GL_QUADS);{

            glVertex3f(a,a,0) ; 
            glVertex3f(a,-a,0) ; 
            glVertex3f(-a,-a,0) ; 
            glVertex3f(-a,a,0) ; 

        }glEnd();
}
//! camera position fixed , but change the look up or right vector 
void keyboard(unsigned char key , int a , int b)
{

    position_of_camera.show(); 
    camera_look.show() ; 
    camera_up.show() ; 
    camera_right.show() ;

    switch (key)
    {
    case '2':
        camera_look = RodriGeneral(camera_look , camera_up , 0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , 0.1) ; 
        // to make sure rodr. works , the changed vector's axis needs to be changed at that degree to make sure they are still unit and perpendicular 
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

    position_of_camera.show(); 
    camera_look.show() ; 
    camera_up.show() ; 
    camera_right.show() ;


    switch (key)
    {
    case GLUT_KEY_UP:
        cout<<"aa"<<endl ; 
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
        //printf("aeh?\n") ; 
        break;
    }
}
void idle()
{
    //printf("currently we dont have job \n") ; 
    glutPostRedisplay(); //abar display function call korbe
}

void showCube()
{
    glEnable(GL_DEPTH_TEST) ; // enabled , so now depth buffer will work correctly 
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  //gluLookAt(10,10,10,5,5,5,0,1,0) ;  

    gluLookAt(position_of_camera.getx() , position_of_camera.gety() , position_of_camera.getz() , 
    position_of_camera.getx()+camera_look.getx() , position_of_camera.gety()+camera_look.gety() , position_of_camera.getz()+camera_look.getz() , 
    camera_up.getx() , camera_up.gety() , camera_up.getz()
    ) ; 

    glMatrixMode(GL_MODELVIEW);

    draw_axes() ; 


    for(int i=0 ; i<4; i++)
    {
        glPushMatrix(); 
        setcolor((i+1)%9);
        glRotatef(i*90 , 1,0,0);  // angle and then the axis around which it will be rotated           
        glTranslatef(0,0,2) ; 
        drawSquare(2);
        glPopMatrix();

        

       if(i!=3)
       {
        glPushMatrix();  
        setcolor((i+5)%9); 
        glRotatef((i+1)*90 ,0,1,0);             
        glTranslatef(0,0,2) ;  
        drawSquare(2);
        glPopMatrix();
    
       }

    
    }
    glutSwapBuffers() ; 
 

}

int main(int argc , char** argv)
{
    glutInit(&argc, argv) ; 

    //- Set up the glut window
    glutInitWindowPosition(1000,200) ; 
    glutInitWindowSize(600,600) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; //RGB - Double buffer - depth buffer 
    glutCreateWindow("Camera Movement") ; 



    init()  ; 



    glutIdleFunc(idle) ; // when no display 
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard); 
    glutDisplayFunc(showCube); // display 
    glutMainLoop(); // main loop 
    return 0 ; 
}