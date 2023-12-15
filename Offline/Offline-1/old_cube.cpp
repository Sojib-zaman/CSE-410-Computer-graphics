#include <bits/stdc++.h>
using namespace std ; 
#include <ctime>
#include <cstdlib>
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>
#define pi (2*acos(0.0))
class point
{
    double x ; 
    double y ; 
    double z ; 
    public : 
    point()
    {
        x=0 ; 
        y=0 ; 
        z=0;
    }
    point(double a ,double b , double c)
    {
        x=a ; 
        y=b; 
        z=c ; 

    }
    double getx(){return x;} 
    double gety(){return y;} 
    double getz(){return z;} 

    void setx(double a){x=a;}
    void sety(double a){y=a;}
    void setz(double a){z=a;}

    point addition(point p)
    {
        point newpoint(x+p.getx() , y+p.gety() , z+p.getz()) ; 
        return newpoint ; 
    }
    
    point subtraction(point p)
    {
        point newpoint(x-p.getx() , y-p.gety() , z-p.getz()) ; 
        return newpoint ; 
    }

   point scalarMul(double n)
    {
        point newpoint(x*n , y*n, z*n) ; 
        return newpoint ; 
    }

   point crossMul(point p)
    {
        point newpoint(y*p.getz()-z*p.gety() ,z*p.getx()-x*p.getz() , x*p.gety()-y*p.getx() ) ; 
        return newpoint ; 
    }


    


}; 
point position_of_camera ; 
point camera_up ; 
point camera_look ; 
point camera_right ; 
double r_factor = 0.0 ; 
point RodriGeneral(point rotatingVector , point withRespectTo , double angle)
{
    point perpendicular ; 
    perpendicular = rotatingVector.crossMul(withRespectTo) ; 
    point newpoint ; 
    newpoint = rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle))) ; 
    return newpoint ; 

}
void keyboard(unsigned char key , int a , int b)
{
    switch (key)
    {
    case '1':
        camera_look = RodriGeneral(camera_look , camera_up , 0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , 0.1) ; 
        // to make sure rodr. works , the changed vector's axis needs to be changed at that degree to make sure they are still unit and perpendicular 
        break;
    case '2':
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
    case '5':
        camera_right = RodriGeneral(camera_right , camera_look , 0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , 0.1) ; 
        break;
    case '6':
        camera_right = RodriGeneral(camera_right , camera_look , -0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , -0.1) ; 
        break;
    case '.':
        r_factor-=1.0/10.0 ; 
        if(r_factor<=0.0)r_factor=0.0;
        cout<<r_factor<<endl ; 
        break;
    case ',':
        r_factor+=1.0/10.0 ; 
        if(r_factor>=1.0)r_factor=1.0;
        cout<<r_factor<<endl ; 
        break;

    default:
        break;
    }
}


//! Changes the camera position only but not the look up or right vectors
void keystrokehandler(int key , int x  , int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        cout<<"aa"<<endl ; 
        position_of_camera=position_of_camera.addition(camera_look) ; 
        break;
    case GLUT_KEY_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_look) ; 
        break;
    case GLUT_KEY_RIGHT:
        position_of_camera=position_of_camera.addition(camera_right) ; 
        break;
    case GLUT_KEY_LEFT:
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
    position_of_camera=point(8.0,8.0,8.0) ; 

    camera_up=point(3.0/sqrt(18.0) ,0 , -3.0/sqrt(18.0) ) ; 
    camera_right=point(-1.0/sqrt(6.0) , 2.0/sqrt(6.0) , -1.0/sqrt(6.0) );
    camera_look=point(-1.0/sqrt(3.0) , -1.0/sqrt(3.0) , -1.0/sqrt(3.0));

    // camera_up=point(0,0,1.0) ; 
    // camera_right=point(-1.0/sqrt(2.0) , 1.0/sqrt(2.0) , 0.0 );
    // camera_look=point(-1.0/sqrt(2.0) , -1.0/sqrt(2.0) , 0.0);
}

void drawTriangle(int length)
{
    glBegin(GL_TRIANGLES) ; 
    //glColor3f(1,0,0) ; 
    glVertex3f(0,0,length) ; 
    glVertex3f(0,length,0) ;    
    glVertex3f(length,0,0) ;
    glEnd() ;  
}


void drawUpperHalf()
{
    for(int i=0 ; i<4 ; i++)
    {
        if(i%2==0)glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        else glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glPushMatrix() ; 
        glRotatef(i*90 , 0 , 0 , 1) ; 
        glTranslatef(r_factor/3.0,r_factor/3.0,r_factor/3.0) ; 
       // glTranslatef(1.0/3.0,1.0/3.0,1.0/3.0) ; 
        glScalef(1-r_factor,1-r_factor,1-r_factor) ; 
        drawTriangle(5) ; 
   
        glPopMatrix() ; 
    }

       
}
void drawLowerHalf()
{
    for(int i=0 ; i<4 ; i++)
    {
        if(i%2==1)glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        else glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glPushMatrix() ;
        
        glRotatef(180.0, 1, 1, 0); 
        glRotatef(i*90 , 0 , 0 , 1) ; 
        glTranslatef(r_factor/3.0,r_factor/3.0,r_factor/3.0) ;
        //glTranslatef(1.0/3.0,1.0/3.0,1.0/3.0) ; 
        glScalef(1-r_factor,1-r_factor,1-r_factor) ; 
        drawTriangle(5) ; 
        glPopMatrix() ;
    }

       
}



void drawOctaHedron()
{

    drawUpperHalf() ; 
    drawLowerHalf() ;
 

    
    

}

struct Sphere_point
{
	double x,y,z;
};

// Taken direct from the main.cpp provided 
void drawSphere(double radius,int slices,int stacks)
{
	struct Sphere_point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
                glColor3f(1.0 , 0.0 , 0.0)  ; 
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glColor3f(0.0 , 0.0 , 1.0)  ; 
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
void drawCylinder()
{
    
}

void magic_cube()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); // Now working on model view matrix 
    glLoadIdentity(); // all previous matrix cleared 

    gluLookAt(position_of_camera.getx() , position_of_camera.gety() , position_of_camera.getz() , 
    position_of_camera.getx()+camera_look.getx() , position_of_camera.gety()+camera_look.gety() , position_of_camera.getz()+camera_look.getz() , 
    camera_up.getx() , camera_up.gety() , camera_up.getz()
    ) ; 
    

 


    glPushMatrix() ; 
    //glRotatef(90,0,1,0) ; 
    drawOctaHedron() ; 
    drawSphere(2,24,20); 
    drawCylinder() ; 
    glPopMatrix() ; 
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
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard); 
    glutDisplayFunc(magic_cube) ;
    glutIdleFunc(idle) ;
    glutMainLoop() ; 
    return 0 ; 
}