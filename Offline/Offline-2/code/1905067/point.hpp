#include<bits/stdc++.h>
#include<math.h>
using namespace std ; 
class point
{
    public:
        double x,y,z,w ; 
        
        point()
        {
            x=y=z=0 ;
            w=1; 
        }
        point(double a,  double b , double c , double d=1)
        {
            x=a;
            y=b;
            z=c;
            w=d;
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

        double dotproduct(point p)
        {
            return x*p.getx()+y*p.gety()+z*p.getz() ; 
        }

        point crossproduct(point p)
        {
            point newpoint(y*p.getz()-z*p.gety() ,z*p.getx()-x*p.getz() , x*p.gety()-y*p.getx() ) ; 
            return newpoint ; 
        }


        void show(string str="KEYBOARD")
        {
            cout<<str<<" "<<x<<","<<y<<","<<z<<","<<w<<"---"<<endl ; 
        }

        point normalize()
        {
            double value = sqrt(x*x+y*y+z*z) ; 
            return point(x/value , y/value , z/value ) ; 
        }

};