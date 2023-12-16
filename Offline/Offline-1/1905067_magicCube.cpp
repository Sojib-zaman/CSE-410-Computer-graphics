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
double r_factor = 1.0 ;
#define red 1
#define blue 2
#define green 3
#define yellow 4
#define cyan 5
#define magenta 6
#define black 7
#define white 8
double co_ord = 1.0 ; 

double triangle_length()
{
    return sqrt(2*co_ord*co_ord) ; 
}

double Cy_height = sqrt(2);
double Cy_radius = 1.0/sqrt(3) ;









void setcolor(int color)
{
    switch (color)
    {
    case 1:
        glColor3f(1.0,0.0,0.0) ; 
        break;
    case 2:
        glColor3f(0.0,0.0,1.0) ; 
        break;
    case 3:
        glColor3f(0.0,1.0,0.0) ; 
        break;
    case 4:
        glColor3f(1.0,1.0,0.0) ; 
        break;
    case 5:
        glColor3f(0.0f, 1.0f, 1.0f); 
        break;
    case 6:
        glColor3f(1.0f, 0.0f, 1.0f);
        break;
    case 7:
        glColor3f(0.0f, 0.0f, 0.0f);
        break;
    case 8:
        glColor3f(1.0f, 1.0f, 1.0f);
        break;
    default:
        printf("aeh?\n") ; 
        break;
    }

}



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
    position_of_camera=point(3,3,3) ; 

    camera_up=point(3.0/sqrt(18.0) ,0 , -3.0/sqrt(18.0) ) ; 
    camera_right=point(-1.0/sqrt(6.0) , 2.0/sqrt(6.0) , -1.0/sqrt(6.0) );
    camera_look=point(-1.0/sqrt(3.0) , -1.0/sqrt(3.0) , -1.0/sqrt(3.0));

   
}
// actual length is root 2 (using 1)
void drawTriangle(int co_ord)
{
    glBegin(GL_TRIANGLES) ; 
    //glColor3f(1,0,0) ; 
    glVertex3f(0,0,co_ord) ; 
    glVertex3f(0,co_ord,0) ;    
    glVertex3f(co_ord,0,0) ;
    glEnd() ;  
}


void drawUpperHalf(int n)
{
    
    for(int i=0 ; i<4 ; i++)
    {
        if(i%2==n)glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        else glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glPushMatrix() ; 
        glRotatef(i*90 , 0 , 0 , 1) ; 
        glTranslatef(r_factor/3.0,r_factor/3.0,r_factor/3.0) ; 
        glScalef(1-r_factor,1-r_factor,1-r_factor) ; 
        drawTriangle(co_ord) ; 
   
        glPopMatrix() ; 
    }

       
}
void drawLowerHalf()
{
   glRotatef(180.0, 1, 1, 0); 
   drawUpperHalf(1) ; 

       
}



void drawOctaHedron()
{

    drawUpperHalf(0) ; 
    drawLowerHalf() ;
 

    
    

}



struct Sphere_point
{
	double x,y,z;
};


//! CODE TAKEN FROM THE LINK PROVIDED IN THE SPEC ( MODIFIED IN THE RECEIVING END )
// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}














void drawSphere(int  subdivision)
{
    struct Sphere_point points[100][100];
    vector<float> vertices = buildUnitPositiveX(subdivision);
    unsigned int pointsPerRow = (unsigned int)pow(2, subdivision) + 1;

    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            int index = i * pointsPerRow + j;
            points[i][j].x = vertices[index * 3]* 1/sqrt(3);
            points[i][j].y = vertices[index * 3 + 1]  * 1/sqrt(3);
            points[i][j].z = vertices[index * 3 + 2]  * 1/sqrt(3);
        }
    }

    // this is from main.cpp 
    // Draw quads using generated points
    for (int i = 0; i < pointsPerRow; i++)
    {
        for (int j = 0; j < pointsPerRow; j++)
        {
            glBegin(GL_QUADS);
            {
                // Upper hemisphere
                glVertex3f(points[i][j].x , points[i][j].y , points[i][j].z );
                glVertex3f(points[i][j + 1].x , points[i][j + 1].y , points[i][j + 1].z );
                glVertex3f(points[i + 1][j + 1].x , points[i + 1][j + 1].y , points[i + 1][j + 1].z );
                glVertex3f(points[i + 1][j].x , points[i + 1][j].y, points[i + 1][j].z);
            }
            glEnd();
        }
    }
}
void drawHalfS(int color , point axis)
{  
    
    for(int i=0 ; i<=2 ; i+=2)
    {

        glPushMatrix() ; 
        setcolor(color) ;
        glRotatef(90*i,axis.getx() , axis.gety() , axis.getz()) ;
        glTranslatef(1.0-r_factor, 0.0, 0.0);
        glScalef(r_factor,r_factor,r_factor);
        drawSphere(3);
        glPopMatrix() ; 

    }
}

void drawSphereStructure()
{ 
glPushMatrix() ; 
    drawHalfS(red , point(0,1,0)) ; 
glPopMatrix() ;
glPushMatrix() ;
    glRotatef(90,0,1,0) ;
    drawHalfS(green, point(0,1,0)) ; 
glPopMatrix() ;
  
glPushMatrix() ;
    glRotatef(90,0,0,1) ;
    drawHalfS(blue, point(0,1,0)) ; 
glPopMatrix() ;
}
void Cylinder(double h , double r)
{
    int slices = 200 ; 
    double dihedral_angle=109.47 ; 
    double x1=r ; 
    double y1=0 ; 
    double x2 =0 , y2=0 ; 
    glTranslatef((1-r_factor)/sqrt(2) , 0 , 0) ; 
    glRotatef(-(180-dihedral_angle)/2 , 0 , 0 , 1) ; 
    glBegin(GL_QUADS) ; 
    {
        for(int i=1 ; i<= slices ; i++)
        {
            double angle = i*dihedral_angle/slices ; 
            x2=r*cos(angle) ; 
            y2=r*sin(angle);
            glVertex3f(x2,y2,h/2);
            glVertex3f(x2,y2,-h/2); 
            glVertex3f(x1,y1,-h/2);
            glVertex3f(x1,y1,h/2);
            x1=x2;
            y1=y2;
        }
    }
    glEnd() ;
   
}


void drawpairs(double h,double r)
{
    glRotatef(45,0,1,0) ; 
    for(int i=1 ; i<=4 ; i++)
    {
        if(i!=1) glRotatef(90,0,1,0); 
        glPushMatrix() ; 
        Cylinder(h,r) ; 
        glPopMatrix(); 
    }
}
void drawCylinderStructure(double init_r , double init_h)
{
    double h = init_h*(1-r_factor) ;
    double r= init_r*r_factor ;  

    setcolor(yellow) ; 
    glPushMatrix();
    drawpairs(h,r) ;
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,1,0,0);
    drawpairs(h,r) ;
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,0,1);
    drawpairs(h,r) ;
    glPopMatrix();
    
   
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
    drawOctaHedron() ; 
    glPopMatrix() ; 
    glPushMatrix() ; 
    drawSphereStructure() ; 
    glPopMatrix() ; 
    glPushMatrix() ; 
    drawCylinderStructure(Cy_radius , Cy_height) ;
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
    glutInitWindowPosition(800,200) ; 
    glutInitWindowSize(1000,1000) ; 
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