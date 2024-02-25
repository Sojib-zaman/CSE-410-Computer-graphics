#include<bits/stdc++.h>
using namespace std ; 
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2*acos(0.0)) 
#define red 1
#define blue 2
#define green 3
#define yellow 4
#define cyan 5
#define magenta 6
#define black 7
#define white 8


class Vector3D
{
    public:
    double x ; 
    double y ; 
    double z ; 
     
    Vector3D()
    {
        x=0 ; 
        y=0 ; 
        z=0;
    }
    Vector3D(double a ,double b , double c)
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

    Vector3D addition(Vector3D p)
    {
        Vector3D newVector3D(x+p.getx() , y+p.gety() , z+p.getz()) ; 
        return newVector3D ; 
    }
    void setattr(double a , double b , double c)
    {
        x=a ; 
        y=b ; 
        z=c; 
    }
    Vector3D subtraction(Vector3D p)
    {
        Vector3D newVector3D(x-p.getx() , y-p.gety() , z-p.getz()) ; 
        return newVector3D ; 
    }

   Vector3D scalarMul(double n)
    {
        Vector3D newVector3D(x*n , y*n, z*n) ; 
        return newVector3D ; 
    }

   Vector3D crossMul(Vector3D p)
    {
        Vector3D newVector3D(y*p.getz()-z*p.gety() ,z*p.getx()-x*p.getz() , x*p.gety()-y*p.getx() ) ; 
        return newVector3D ; 
    }


        void show(string str="KEYBOARD")
    {
        cout<<str<<" "<<x<<","<<y<<","<<z<<"---"<<endl ; 
    }



}; 
Vector3D position_of_camera ; 
Vector3D camera_up ; 
Vector3D camera_look ; 
Vector3D camera_right ; 



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
Vector3D RodriGeneral(Vector3D rotatingVector , Vector3D withRespectTo , double angle)
{
    Vector3D perpendicular ; 
    perpendicular = rotatingVector.crossMul(withRespectTo) ; 
    Vector3D newVector3D ; 
    newVector3D = rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle))) ; 
    return newVector3D ; 

}

class Object {
public:
    Vector3D reference_point;
    double height, width, length;
    double color[3];
    double ambient ; 
    double diffuse ;
    double specular ;
    double reflection ;

    //-double coEfficients[4]; //ambient,diffuse,specular,reflection coefficients
    int shine; // exponent term of specular component


    Object() {}

    virtual void draw() {}

    void setColor() {

    }

    void setShine() {

    }

    void setCoEfficients() {

    }
};

struct Sphere : public Object {
    Vector3D center;
    double radius;
    Sphere() {
        center = Vector3D(0, 0, 0);
        radius = 0;
    }
    Sphere(Vector3D center, double radius) {
        this->center = center;
        this->radius = radius;
    }
    Sphere(Vector3D center, double radius , double color[3] , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->center = center;
        this->radius = radius;
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 

    }

    void draw() {
        cout << "Drawing a sphere..." << endl;
    }

    friend istream &operator>>(istream &input, Sphere &s) {
        input >> s.center.x >> s.center.y >> s.center.z >> s.radius >> s.color[0] >> s.color[1] >> s.color[2] >> s.ambient >> s.diffuse >> s.specular >> s.reflection >> s.shine;
        return input;
    }
    void showSphereInformation()
    {
        cout<<"Sphere Information"<<endl ; 
        cout<<"Center: "<<center.x<<","<<center.y<<","<<center.z<<endl ; 
        cout<<"Radius: "<<radius<<endl ; 
        cout<<"Color: "<<color[0]<<","<<color[1]<<","<<color[2]<<endl ; 
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }
};
struct Triangle : public Object
{
    Vector3D p1, p2, p3;
    Triangle(){
        p1 = Vector3D(0,0,0);
        p2 = Vector3D(0,0,0);
        p3 = Vector3D(0,0,0);
    } 
    Triangle(Vector3D p1, Vector3D p2, Vector3D p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
    Triangle(Vector3D p1, Vector3D p2, Vector3D p3 , double color[3] , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 
    }
    friend istream &operator>>(istream &input, Triangle &t)
    {
        input >> t.p1.x >> t.p1.y >> t.p1.z >> t.p2.x >> t.p2.y >> t.p2.z >> t.p3.x >> t.p3.y >> t.p3.z >> t.color[0] >> t.color[1] >> t.color[2] >> t.ambient >> t.diffuse >> t.specular >> t.reflection >> t.shine;
        return input;
    }
    void showTriangleInformation()
    {
        cout<<"Triangle Information"<<endl ; 
        cout<<"P1: "<<p1.x<<","<<p1.y<<","<<p1.z<<endl ; 
        cout<<"P2: "<<p2.x<<","<<p2.y<<","<<p2.z<<endl ; 
        cout<<"P3: "<<p3.x<<","<<p3.y<<","<<p3.z<<endl ; 
        cout<<"Color: "<<color[0]<<","<<color[1]<<","<<color[2]<<endl ; 
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }
};
class PointLight {
    public:
    Vector3D light_position;
    double color[3];
    PointLight() {
        light_position = Vector3D(0, 0, 0);
    }
    PointLight(Vector3D poisition , double color[3])
    {
        this->light_position = poisition ; 
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
    }
    friend istream &operator>>(istream &input , PointLight &pl)
    {
        input>>pl.light_position.x>>pl.light_position.y>>pl.light_position.z>>pl.color[0]>>pl.color[1]>>pl.color[2] ; 
        return input ; 
    }
    void showPointLightInformation()
    {
        cout<<"Point Light Information"<<endl ; 
        cout<<"Position: "<<light_position.x<<","<<light_position.y<<","<<light_position.z<<endl ; 
        cout<<"Color: "<<color[0]<<","<<color[1]<<","<<color[2]<<endl ; 
    
    }

};
class SpotLight {
    public:
    PointLight point_light ;
    Vector3D light_direction ;
    double cutoff_angle ; 
    SpotLight()
    {
        point_light = PointLight() ; 
        light_direction = Vector3D(0,0,0) ; 
        cutoff_angle = 0 ; 
    
    }
    SpotLight(PointLight pl , Vector3D ld , double c_angle)
    {
        point_light = pl  ; 
        light_direction = ld ;
        cutoff_angle = c_angle ;
    }
    friend istream &operator>>(istream &input , SpotLight &sl)
    {
        input>>sl.point_light>>sl.light_direction.x>>sl.light_direction.y>>sl.light_direction.z>>sl.cutoff_angle ; 
        return input ; 
    }
    void showSpotLightInformation()
    {
        cout<<"Spot Light Information"<<endl ; 
        point_light.showPointLightInformation() ; 
        cout<<"Light Direction: "<<light_direction.x<<","<<light_direction.y<<","<<light_direction.z<<endl ; 
        cout<<"Cutoff Angle: "<<cutoff_angle<<endl ; 
    
    }

};
extern vector<Object*> objects ;
extern vector<PointLight> point_lights ;
extern vector<SpotLight> spot_lights ;
