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
        glBegin(GL_LINES) ;
    {
        glColor3f(1,0,0);
        glVertex2f(-1.0f , 0.0f) ; 
        glVertex2f(1 , 0.0f) ; 

        glColor3f(1,1,0);
        glVertex2f(0.0f , 1.0f) ; 
        glVertex2f(0.0f , -1) ; 

    }glEnd();
}

void drawSquare(double a)
{
     glBegin(GL_QUADS);{

            
            glVertex2f(a,a) ; 
            glVertex2f(a,-a) ; 
            glVertex2f(-a,-a) ; 
            glVertex2f(-a,a) ; 

        }glEnd();
}

int animate = 0  ; 

void display()
{
    
    printf("we are in display : %d\n" , counter) ;
    
    
    glClear(GL_COLOR_BUFFER_BIT);

    axes() ; 
    //drawSquare(0.25);

    // double x[]={0.5 , 0.5 , -0.5 , -0.5} ; 
    // double y[]={0.5 , -0.5 , -0.5 , 0.5};


    // for(int i=0 ; i<4 ; i++)
    // {


        glPushMatrix();
        //glRotatef(90*i,0,0,1) ; 

        if(animate)
        {
            glRotatef(counter%360 , 0, 0,1);
            
        }
        glTranslatef(.5,0.5,0) ; 

        glColor3f(1,0,0) ; 
        //glTranslatef(x[i],y[i],0);
        drawSquare(0.25);
        //glTranslatef(-x[i] , -y[i] , 0 ) ; 
        glPopMatrix();
    //}
   


    glFlush(); //sends update to display from buffer
}

void init()
{
    animate=1;
    glClearColor(0.0f,0.0f,1.0f,1.0f) ; 
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
    glutInitWindowPosition(100,200) ; 
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