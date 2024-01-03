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
#define posX 100
#define negX 101 
#define posY 102
#define negY 103

class point
{
    public:
    double x ; 
    double y ; 
    double z ; 
     
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
    void setattr(double a , double b , double c)
    {
        x=a ; 
        y=b ; 
        z=c; 
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


        void show(string str="KEYBOARD")
    {
        cout<<str<<" "<<x<<","<<y<<","<<z<<"---"<<endl ; 
    }



}; 
struct Sphere_point
{
	double x,y,z;
    public:
    Sphere_point scalarMul(double n)
    {
        Sphere_point newpoint;
        newpoint.x=x*n  ; 
        newpoint.y=y*n  ; 
        newpoint.z=z*n  ;          
        return newpoint ; 
    }

   Sphere_point crossMul(point p)
    {
        Sphere_point newpoint ; 
        newpoint.x=y*p.getz()-z*p.gety()  ; 
        newpoint.y=z*p.getx()-x*p.getz() ; 
        newpoint.z=x*p.gety()-y*p.getx()   ; 
        return newpoint ; 
    }
    Sphere_point addition(Sphere_point p)
    {
        Sphere_point newpoint;
        newpoint.x=x+p.x  ; 
        newpoint.y=y+p.y   ; 
        newpoint.z=z+p.z  ;          
        return newpoint ; 
    }

};
point position_of_camera ; 
point camera_up ; 
point camera_look ; 
point camera_right ; 
point RodriGeneral(point rotatingVector , point withRespectTo , double angle)
{
    point perpendicular ; 
    perpendicular = rotatingVector.crossMul(withRespectTo) ; 
    point newpoint ; 
    newpoint = rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle))) ; 
    return newpoint ; 

}
Sphere_point RodriSpGeneral(Sphere_point rotatingVector , point withRespectTo , double angle)
{
    Sphere_point crs = rotatingVector.crossMul(withRespectTo) ; 
    double dotxx = rotatingVector.x * withRespectTo.x +  rotatingVector.y * withRespectTo.y + rotatingVector.z * withRespectTo.z ;
    Sphere_point x = {
        rotatingVector.x * cos(angle) + crs.x * sin(angle) + withRespectTo.x*dotxx*(1-cos(angle)) ,
        rotatingVector.y * cos(angle) + crs.y * sin(angle) + withRespectTo.y*dotxx*(1-cos(angle)) ,
        rotatingVector.z * cos(angle) + crs.z * sin(angle) + withRespectTo.z*dotxx*(1-cos(angle)) 
    };
    return x ;
}
struct Sphere_point points[100][100]; // done this as global to store the points everytime 
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
class Event 
{
    public:
        double eventScheduledTime ;
        int collision_count ; 
        int wall_side ; 
    Event()
    {
        eventScheduledTime = 0 ; 
        collision_count = 0 ; 
        wall_side=-1 ;
    }
    Event(double a , int b , int w )
    {
        eventScheduledTime = a ; 
        collision_count = b ; 
        wall_side = w; 
    }
   

    bool operator<(const Event& other) const {
        return eventScheduledTime > other.eventScheduledTime; 
    }




};

priority_queue<Event> Event_PQ ;
void showPQ() {
    std::priority_queue<Event> tempQueue = Event_PQ; 

    while (!tempQueue.empty()) {
        Event currentEvent = tempQueue.top();

        //cout << "Time: " << currentEvent.eventScheduledTime<< ", Collision Count: " << currentEvent.collision_count << std::endl;

        tempQueue.pop();  
    }
}