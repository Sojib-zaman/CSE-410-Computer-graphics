#include <bits/stdc++.h>
using namespace std ; 

#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
int counter = 0 ;

void axes()
{
   glColor3f(1.0,1.0,1.0) ; 
   glBegin(GL_LINES);
   {
    glVertex3f(-100,0,0) ; 
    glVertex3f(100,0,0) ; 

    glVertex3f(0,-100,0) ; 
    glVertex3f(0,100,0) ; 


    glVertex3f(0,0,-100) ; 
    glVertex3f(0,0,100) ; 

   }glEnd() ;   
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

int animate = 0  ; 

void display()
{

    //glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // eye - AT - up 
    gluLookAt(10,10,5,0,0,0,0,1,0) ; 


    axes() ; 
    


    glColor3f(1,0,1) ; 
    for(int i=0 ; i<4; i++)
    {
        glPushMatrix();  
        glRotatef(i*90 , 1,0,0);             
        glTranslatef(0,0,2) ; 
        glColor3f(1,0,0) ; 
        drawSquare(2);
        glPopMatrix();

        // without using depth test , we can't understand konta shamne konta piche 


        glPushMatrix();  
        glRotatef(i*90 ,0,1,0);             
        glTranslatef(0,0,2) ; 
        glColor3f(0,0,1) ; 
        drawSquare(2);
        glPopMatrix();
    
    
    }

 
   


    //glFlush(); //sends update to display from buffer
    glutSwapBuffers(); // swap the buffers , b2 starts computing and b1 dies flush 

}

void init()
{
    animate=1;
    glMatrixMode(GL_PROJECTION) ; 
    glLoadIdentity(); 
    gluPerspective(60,1,1,100); // this will build a projection matrix 
    //after view transformation(set up camera) , Mview * Mprojection will set up the final output 


}


void idle()
{
    printf("currently we dont have job \n") ; 

    glutPostRedisplay(); //abar display function call korbe
}
void timer(int value)
{
    if(animate)
       {
         counter++ ; 
         glutPostRedisplay(); 
       } 
    glutTimerFunc(100,timer,0);

}


void keystrokehandler(unsigned char key , int x  , int y)
{
    switch (key)
    {
    case 'V':
        printf("V\n") ; 
        animate=0;
        break;
    case 'A':
        printf("V\n") ; 
        animate=1;
        break;
    default:
        printf("aeh?\n") ; 
        break;
    }
}
int main(int argc , char** argv)
{
    cout<<"Hello world"<<endl ; 
    glutInit(&argc ,argv ) ; 
    glutInitWindowPosition(1000,200) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; 
    glutInitWindowSize(500,500) ; 
    glutCreateWindow("opengl demo") ; 

    init() ; 

    glutDisplayFunc(display) ; 
    //glutIdleFunc(idle) ; 
    glutKeyboardFunc(keystrokehandler) ; 
    glutTimerFunc(1420,timer,0);
    glutMainLoop();
    return 0  ; 
}