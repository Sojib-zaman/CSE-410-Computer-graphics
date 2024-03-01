#include<bits/stdc++.h>
using namespace std ; 
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2*acos(0.0)) 
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOE 4
#define CYAN 5
#define MAGENTA 6
#define BLACK 7
#define WHITE 8
#define epsilon 0.0000001


double determinant(double a[3][3])
{
    double ans = 0;
    for(int i=0;i<3;i++)
    {
        ans += a[0][i]*(a[1][(i+1)%3]*a[2][(i+2)%3] - a[1][(i+2)%3]*a[2][(i+1)%3]);
    }
    return ans;
}


class Vector3D ;
class Ray ;
class Object ;
class Light ;
class PointLight ;
class SpotLight ;
class Sphere ;
class Triangle ;
class GeneralQuadSurface ;
class Floor ;
class Color ; 







extern int recursion_level;
extern vector<Object*> objects ;
extern vector<PointLight> point_lights ;
extern vector<SpotLight> spot_lights ;




double convert_to_degree(double radian)
{
    return radian*180/pi;
}

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
    double dotMul(Vector3D p)
    {
        return x*p.getx()+y*p.gety()+z*p.getz() ;
    }

    void show(string str="KEYBOARD")
    {
        cout<<str<<" "<<x<<","<<y<<","<<z<<"---"<<endl ; 
    }

    void normalize()
    {
        double length = sqrt(x*x+y*y+z*z) ; 
        x=x*1.0/length ; 
        y=y*1.0/length ; 
        z=z*1.0/length ; 
    
    }
    double distance(Vector3D p)
    {
        return sqrt((x-p.getx())*(x-p.getx())+(y-p.gety())*(y-p.gety())+(z-p.getz())*(z-p.getz())) ; 
    }


}; 

class Color
{
    public:
    double r ; 
    double g ; 
    double b ; 
    Color()
    {
        r=0 ; 
        g=0 ; 
        b=0 ; 
    }
    Color(double aa , double ba , double ca)
    {
        r=aa ; 
        g=ba ; 
        b=ca ; 
    }
    void setcolor(double aa , double ba , double ca)
    {
        r=aa ; 
        g=ba ; 
        b=ca ; 
    }
    void show()
    {
        cout<<"Color: "<<r<<","<<g<<","<<b<<endl ; 
    }
    void setcolor(Color color)
    {
        r = color.r ; 
        g = color.g ; 
        b = color.b ;
    }
    // NORMALIZE , ADDITION , MULTIPLICATION , SUBTRACTION
    void normalize()
    {
        r = max(0.0,min(1.0,r)) ; 
        g = max(0.0,min(1.0,g)) ; 
        b = max(0.0,min(1.0,b)) ; 
    }
    Color addition(Color c)
    {
        r+=c.r ; 
        g+=c.g ; 
        b+=c.b ; 
        normalize() ; 
        return *this ;
    }
    Color multiplication(double value)
    {
        r*=value ; 
        g*=value ; 
        b*=value ; 
        normalize() ; 
        return *this ;
    }
    Color multiplication(Color c)
    {
        r*=c.r ; 
        g*=c.g ; 
        b*=c.b ; 
        normalize() ; 
        return *this ;
    }
    Color addition(double value)
    {
        r+=value ; 
        g+=value ; 
        b+=value ; 
        normalize() ; 
        return *this ;
    }
};



Vector3D position_of_camera ; 
Vector3D camera_up ; 
Vector3D camera_look ; 
Vector3D camera_right ; 





class Ray
{
public:
    Vector3D start ; // this is Ro
    Vector3D direction ; // this is Rd (will be normalized later)
    Ray()
    {
        start = Vector3D(0,0,0); 
        direction = Vector3D(0,0,0) ;
    }
    Ray(Vector3D a , Vector3D b){
        start = a ; 
        direction = b ; 
        direction.normalize() ; 
    }
};


Vector3D RodriGeneral(Vector3D rotatingVector , Vector3D withRespectTo , double angle)
{
    Vector3D perpendicular ; 
    perpendicular = rotatingVector.crossMul(withRespectTo) ; 
    Vector3D newVector3D ; 
    newVector3D = rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle))) ; 
    return newVector3D ; 

}
void drawSphere(double radius , Color color,int slices , int stacks)
    {
        Vector3D SpherePoints[100][100] ; 
        double h,r;
        for(int i=0;i<=stacks;i++)
        {
            
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(int j=0;j<=slices;j++)
            {
                SpherePoints[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                SpherePoints[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                SpherePoints[i][j].z=h;
            }
        }
        for(int i=0;i<stacks;i++)
        {
            for(int j=0;j<slices;j++)
            {
    
                glBegin(GL_QUADS);{ 
                    glColor3f(color.r,color.g,color.b);
                    
               
                    glVertex3f(SpherePoints[i][j].x,SpherePoints[i][j].y,SpherePoints[i][j].z);
                    glVertex3f(SpherePoints[i][j+1].x,SpherePoints[i][j+1].y,SpherePoints[i][j+1].z);
                    glVertex3f(SpherePoints[i+1][j+1].x,SpherePoints[i+1][j+1].y,SpherePoints[i+1][j+1].z);
                    glVertex3f(SpherePoints[i+1][j].x,SpherePoints[i+1][j].y,SpherePoints[i+1][j].z);

            
                    glVertex3f(SpherePoints[i][j].x,SpherePoints[i][j].y,-SpherePoints[i][j].z);
                    glVertex3f(SpherePoints[i][j+1].x,SpherePoints[i][j+1].y,-SpherePoints[i][j+1].z);
                    glVertex3f(SpherePoints[i+1][j+1].x,SpherePoints[i+1][j+1].y,-SpherePoints[i+1][j+1].z);
                    glVertex3f(SpherePoints[i+1][j].x,SpherePoints[i+1][j].y,-SpherePoints[i+1][j].z);
                }glEnd();
            }
        }

    }

class PointLight{
    public:
    Vector3D light_position;
    Color PointLightColor;
    PointLight() {
        
        light_position = Vector3D(0, 0, 0);
      

    }
    PointLight(Vector3D poisition , Color c)
    {
        this->light_position = poisition ; 
        this->PointLightColor = c ; 
    }
    friend istream &operator>>(istream &input , PointLight &pl)
    {
        input>>pl.light_position.x>>pl.light_position.y>>pl.light_position.z>>pl.PointLightColor.r>>pl.PointLightColor.g>>pl.PointLightColor.b; 
        return input ; 
    }
    void showPointLightInformation()
    {
        cout<<"Point Light Information"<<endl ; 
        cout<<"Position: "<<light_position.x<<","<<light_position.y<<","<<light_position.z<<endl ; 
        cout<<"Color: "<<PointLightColor.r<<","<<PointLightColor.g<<","<<PointLightColor.b<<endl ;
    
    }
    void draw()
    {
        glPushMatrix() ; 
        glTranslatef(light_position.x,light_position.y,light_position.z) ;
        drawSphere(.8,PointLightColor,24,24) ;
        glPopMatrix() ;
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
        light_direction.normalize() ; 
      

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
    void draw()
    {
        glPushMatrix() ; 
        glTranslatef(point_light.light_position.x,point_light.light_position.y, point_light.light_position.z) ;
        drawSphere(2,point_light.PointLightColor,24,24) ;
        glPopMatrix() ;
    }

};

class Object {
public:
    Vector3D reference_point;
    double height, width, length;
    Color color;
    double ambient ; 
    double diffuse ;
    double specular ;
    double reflection ;
    string type ;

    int shine; // exponent term of specular component


    Object() {}

    virtual void draw() = 0;

    void setColor() {

    }

    void setShine() {

    }

    void setCoEfficients() {

    }
    virtual Color getColorAt(Vector3D intersectionPoint)=0 ;
    virtual Vector3D getNormal(Vector3D IntersectionPoint)=0;
    virtual double calculate_t(Ray &r , string str) = 0 ;




    void calc_reflection(Vector3D &L , Vector3D &normal , Vector3D &intersectionPoint , Color &color , int level)
    {
        int nearestOBJ = -1 ; 
        double MinimumT = 1000000 ;
        int objCount = 0 ; 
        Color reflectedColor ;
        Vector3D R = L.subtraction(normal.scalarMul(2*L.dotMul(normal))) ;
        R.normalize() ; 
        // actually slightly forward from the point (by moving the
        // start a little bit towards the reflection direction)
        // to avoid self intersection
        Vector3D start = intersectionPoint.addition(R.scalarMul(epsilon)) ;
        Ray reflectedRay(start,R) ;
        for(Object *o : objects)
        {
            double t = o->intersect(reflectedRay,reflectedColor,0) ; //find the nearest object
            if(t>0 && t<MinimumT)
            {
                MinimumT = t ;
                nearestOBJ = objCount ;
            }
            objCount++ ; 
        }
        if(nearestOBJ!=-1) //so found  
        {
            MinimumT =  objects[nearestOBJ]->intersect(reflectedRay,reflectedColor,level+1) ;
            color=color.addition(reflectedColor.multiplication(reflection)) ;
        }

    }



    //following the intersect pseudo code given on spec
    double intersect(Ray &r , Color &current_color , int level)
    {
        
        double MinimumT = calculate_t(r , this->type) ;
        
        if(level==0) return MinimumT ; 
        
        Vector3D intersectionPoint = findInterSectionPoint(r,MinimumT) ;
        Color IPcolor = getColorAt(intersectionPoint) ;
        current_color = IPcolor.multiplication(ambient) ;
        Vector3D normal = getNormal(intersectionPoint) ;

       



        //cout<<"starting lights"<<endl ; 
        vector<PointLight> all_lights ;
        for(int i=0 ; i<point_lights.size() ; i++)
        {
            all_lights.push_back(point_lights[i]) ; 
        }   
        // If spot light is not in the direction of the intersection point then we will not consider it
        for(int i=0 ; i<spot_lights.size() ; i++)
        {
            Vector3D directionOfSpotLight = spot_lights[i].light_direction ;
            Vector3D SpotLightToIntersectionPoint = intersectionPoint.subtraction(spot_lights[i].point_light.light_position) ;
            directionOfSpotLight.normalize() ; 
            SpotLightToIntersectionPoint.normalize() ; 
            double createdAngle = acos(directionOfSpotLight.dotMul(SpotLightToIntersectionPoint)) ;
            if(convert_to_degree(createdAngle) <=spot_lights[i].cutoff_angle) all_lights.push_back(spot_lights[i].point_light) ; 
        
        }
      
        for(int i=0 ; i<all_lights.size() ; i++)
        {
            
            Vector3D L , R , V , N ;
            L = all_lights[i].light_position.subtraction(intersectionPoint) ;
            N = normal ;  
            R = N.scalarMul(2*(L.dotMul(N))).subtraction(L) ;
            V = position_of_camera.subtraction(intersectionPoint) ;
            Ray incidentRay(all_lights[i].light_position,L.scalarMul(-1)) ; 
            //the ray from intersection point to reflection direction 
            Ray reflectedRay(intersectionPoint,R) ;

            L.normalize() ;
            V.normalize() ;
            double distanceFromIntersectionToLight = intersectionPoint.distance(all_lights[i].light_position) ;
            
            //sending light from light source to the intersection point 
        
            double LdotN = L.dotMul(N) ;
            double RdotV = reflectedRay.direction.dotMul(r.direction) ;


            // now we have to check if the incident ray intersects with any object or not before reaching the intersection point
            // if it intersects then we will not consider the light source
           
            bool isBlocked = false ;
            double t2, min_t = 1000000 ;
            for(Object *o : objects)
            {
                t2 = o->intersect(incidentRay , current_color , 0) ; 
                if(t2>0 && t2<min_t)
                {
                    min_t = t2 ;
                }
            }
            //cout<<"blocked calculation done"<<endl ; 
            if(min_t < distanceFromIntersectionToLight-epsilon) isBlocked = true ;
            if(!isBlocked)
            {
                double lambert_value = max(0.0,LdotN) ;
                double phong_value = max(0.0,pow(RdotV,shine)) ;
                // cout<<"lambert value "<<lambert_value<<endl ;
                // cout<<"phong value "<<phong_value<<endl ;

                Color temp_color = all_lights[i].PointLightColor.multiplication(getColorAt(intersectionPoint)) ; 
                Color lambert_Component = temp_color.multiplication(lambert_value*diffuse) ;
                temp_color = all_lights[i].PointLightColor.multiplication(getColorAt(intersectionPoint)) ;
                Color phong_Component = temp_color.multiplication(phong_value*specular) ;
                current_color.addition(lambert_Component) ;
                current_color.addition(phong_Component) ;
                current_color.normalize() ;
            }
      
        

        }

        
        //current_color.show();  

        if(level>=recursion_level) return MinimumT;
       
        calc_reflection(r.direction, normal , intersectionPoint , current_color , level) ;
        return MinimumT ; 
    }
    Vector3D findInterSectionPoint(Ray &r , double t)
    {
        return r.start.addition(r.direction.scalarMul(t)) ; 
    }
};

struct Floor: public Object{
    Vector3D initial_point ; 
    double tilewidth ;
    double floorwidth ;  
    Floor()
    {
        initial_point = Vector3D(0,0,0) ; 
        floorwidth = 0 ; 
    }
    Floor(Vector3D initial_point ,double floorwidth ,  double tilewidth , Color color , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->initial_point = initial_point ;
        this->floorwidth = floorwidth ;
        this->tilewidth = tilewidth ; 
        this->color.setcolor(color) ;
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 
    }
    void draw() override
    {

        int floorRow = (int)(floorwidth/tilewidth) ;
        int floorCol = (int)(floorwidth/tilewidth) ; 
        for(int i=0 ; i<floorRow ; i++)
        {
            for(int j=0 ; j<floorCol ; j++)
            {
                if((i+j)%2==0)
                { 
                    glColor3f(color.r,color.g,color.b) ;
                }
                else
                {
                    glColor3f(1-color.r,1-color.g,1-color.b) ; 
                }
                glBegin(GL_QUADS) ; 
                glVertex3f(initial_point.x+i*tilewidth , initial_point.y+j*tilewidth , initial_point.z) ; 
                glVertex3f(initial_point.x+(i+1)*tilewidth , initial_point.y+j*tilewidth , initial_point.z) ; 
                glVertex3f(initial_point.x+(i+1)*tilewidth , initial_point.y+(j+1)*tilewidth , initial_point.z) ; 
                glVertex3f(initial_point.x+i*tilewidth , initial_point.y+(j+1)*tilewidth , initial_point.z) ; 
                glEnd() ; 
            }
        }
    }

    void showFloorInformation()
    {
        cout<<"Floor Information"<<endl ; 
        cout<<"Initial Point: "<<initial_point.x<<","<<initial_point.y<<","<<initial_point.z<<endl ; 
        cout<<"TileWidth: "<<tilewidth<<endl ; 
        cout<<"Floorwidth: "<<floorwidth<<endl ; 
        cout<<"Color: "<<color.r<<","<<color.g<<","<<color.b<<endl; 
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    }
    double calculate_t(Ray &r , string str) override
    {

        Vector3D normal(0,0,1) ; 
        double up = -1.0* (r.start.dotMul(normal)) ;
        double down = r.direction.dotMul(normal) ;
        if(down==0) return -1 ;
        double t = up/down ;
        if(t<0) return -1 ;
        Vector3D intersectPoint = r.start.addition(r.direction.scalarMul(t)) ;
        if(intersectPoint.x<initial_point.x || intersectPoint.x>-initial_point.x) return -1 ;
        if(intersectPoint.y<initial_point.y || intersectPoint.y>-initial_point.y) return -1 ;
        return t ;


    }
    Vector3D getNormal(Vector3D intersectionPoint) override
    {
        Vector3D normal = Vector3D(0,0,1); 
        normal.normalize() ; 
        return normal ; 
    }
    Color getColorAt(Vector3D intersectionPoint) override
    {
        // either black or white , depending on the point 
        // cout<<"normal color " ; 
        // color.show() ;
        Color xx; 
        int r = (int)(abs(intersectionPoint.x - initial_point.x)/tilewidth) ;
        int c = (int)(abs(intersectionPoint.y - initial_point.y)/tilewidth) ;
        if((r+c)%2==0)
        {
            xx.setcolor(color) ; 
            //cout<<"this"<<endl ; 
        }
        else
        {
            double r = 1-color.r ; 
            double g = 1-color.g ;
            double b = 1-color.b ; 
            // cout<<"b :"<<b<<endl ; 
            xx.setcolor(r,g,b) ;
            // cout<<"ffffffffaaaaaaaaaaaaaaaaaa"<<endl  ; 
        }
        // xx.show() ; 
        return xx ;
    }
};


struct Sphere : public Object {
public:
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
    Sphere(Vector3D center, double radius ,Color x)
    {
        this->reference_point = center ; 
        this->center = center;
        this->radius = radius;
        this->color.setcolor(x) ;
    }
    Sphere(Vector3D center, double radius , Color x , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->reference_point = center ; 
        this->center = center;
        this->radius = radius;
        this->color.setcolor(x) ;
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 

    }
    Vector3D getNormal(Vector3D intersectionPoint) override
    {
        Vector3D normal = intersectionPoint.subtraction(center) ; 
        normal.normalize() ; 
        return normal ; 
    }
    Color getColorAt(Vector3D intersectionPoint) override
    {
       // cout<<"IN SPHERE RETURN"<<endl ; 
        //color.show() ;
        Color xx; 
        xx.setcolor(color) ;
        return xx ;
    }

    void draw() {
  
       int slices = 24 ; 
       int stacks = 24 ; 
       glPushMatrix() ; 
       drawSphere(radius , color ,slices , stacks) ; 
       glPopMatrix() ; 
    }

    friend istream &operator>>(istream &input, Sphere &s) {
        input >> s.center.x >> s.center.y >> s.center.z >> s.radius >> s.color.r >> s.color.g >> s.color.b >> s.ambient >> s.diffuse >> s.specular >> s.reflection >> s.shine;
        return input;
    }
    void showSphereInformation()
    {
        cout<<"Sphere Information"<<endl ; 
        cout<<"Center: "<<center.x<<","<<center.y<<","<<center.z<<endl ; 
        cout<<"Radius: "<<radius<<endl ; 
        cout<<"Color: "<<color.r<<","<<color.g<<","<<color.b<<endl ;
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }

    
    double calculate_t(Ray &r , string str) override
    {

        // general equation (slide) will be finally t2*(Rd.Rd) + t*2*R*Rd + R.R-r^2 = 0
        // so Rd.Rd = 1 (since normalized ) , and a=1 
        // b=2*(Rd.R) , c = R.R-r^2
        
        Vector3D CurrStart = r.start.subtraction(reference_point) ; 
        
        double a = 1 ;
        double b = (r.direction.dotMul(CurrStart))*2 ;
        double c = (CurrStart.dotMul(CurrStart)) - radius*radius ;
        double d = b*b - 4*a*c ;
       
        if(d<0) return -1 ;
       
        double t1 = (-b+sqrt(d))/(2*a) ;
        double t2 = (-b-sqrt(d))/(2*a) ;   
        if(t1>0 || t2>0)
        { 
            // cout<<a<<" "<<b<<" "<<c<<" "<<d<<endl ; 
            // cout<<t1<<" "<<t2<<endl ; 
            // cout<<"here "<<str<<endl; 
            // cout<<"ray info : "<<endl ;
            // r.start.show("start") ;
            // r.direction.show("direction") ;
        }

        if(t1<0 && t2<0) return -1 ;
        if(t1<0) return t2 ;
        if(t2<0) return t1 ;
        return min(t1,t2) ;
        



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
    Triangle(Vector3D p1, Vector3D p2, Vector3D p3 , Color color, double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->reference_point = Vector3D(0,0,0) ;
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->color.setcolor(color) ;
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 
    }
    friend istream &operator>>(istream &input, Triangle &t)
    {
        input >> t.p1.x >> t.p1.y >> t.p1.z >> t.p2.x >> t.p2.y >> t.p2.z >> t.p3.x >> t.p3.y >> t.p3.z >> t.color.r >> t.color.g >> t.color.b >> t.ambient >> t.diffuse >> t.specular >> t.reflection >> t.shine;
        return input;
    }
    void showTriangleInformation()
    {
        cout<<"Triangle Information"<<endl ; 
        cout<<"P1: "<<p1.x<<","<<p1.y<<","<<p1.z<<endl ; 
        cout<<"P2: "<<p2.x<<","<<p2.y<<","<<p2.z<<endl ; 
        cout<<"P3: "<<p3.x<<","<<p3.y<<","<<p3.z<<endl ; 
        cout<<"Color: "<<color.r<<","<<color.g<<","<<color.b<<endl ;
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }
    void draw()
    {
        glBegin(GL_TRIANGLES) ; 
        glColor3f(color.r,color.g,color.b) ; 
        glVertex3f(p1.x,p1.y,p1.z) ; 
        glVertex3f(p2.x,p2.y,p2.z) ; 
        glVertex3f(p3.x,p3.y,p3.z) ; 
        glEnd() ; 
    
    }
    double calculate_t(Ray &r , string str) override
    {
        //using barycentric triangle idea 
        double detMatrix[3][3]={
            {p1.x-p2.x,p1.x-p3.x,r.direction.x},
            {p1.y-p2.y,p1.y-p3.y,r.direction.y},
            {p1.z-p2.z,p1.z-p3.z,r.direction.z}
        };
        double betaMatrix[3][3]={
            {p1.x-r.start.x,p1.x-p3.x,r.direction.x},
            {p1.y-r.start.y,p1.y-p3.y,r.direction.y},
            {p1.z-r.start.z,p1.z-p3.z,r.direction.z}
        };
        double gammaMatrix[3][3]={
            {p1.x-p2.x,p1.x-r.start.x,r.direction.x},
            {p1.y-p2.y,p1.y-r.start.y,r.direction.y},
            {p1.z-p2.z,p1.z-r.start.z,r.direction.z}
        };
        double tMatrix[3][3]={
            {p1.x-p2.x,p1.x-p3.x,p1.x-r.start.x},
            {p1.y-p2.y,p1.y-p3.y,p1.y-r.start.y},
            {p1.z-p2.z,p1.z-p3.z,p1.z-r.start.z}
        };
        double detMatrixD = determinant(detMatrix) ;
        if(detMatrixD==0) return -1 ;
        double beta , gamma , t ; 
        beta = determinant(betaMatrix)/detMatrixD ;
        gamma = determinant(gammaMatrix)/detMatrixD ;
        t = determinant(tMatrix)/detMatrixD ;
        if(beta>=0 && gamma>=0 && beta+gamma<=1 && t>0) return t ;
        return -1 ;



        

    }
    Vector3D getNormal(Vector3D intersectionPoint) override
    {
        // normal = (p2-p1)X(p3-p1)
        Vector3D normal = (p2.subtraction(p1)).crossMul(p3.subtraction(p1)) ;
        normal.normalize() ; 
        return normal ; 
    }
    Color getColorAt(Vector3D intersectionPoint) override
    {    
        Color xx; 
        xx.setcolor(color) ;
        return xx ;
    }
    
};




struct GeneralQuadSurface : public Object
{
    double a,b,c,d,e,f,g,h,i,j ; 
    Vector3D cube_reference_point ;
    double length , width , height ;
    GeneralQuadSurface()
    {
        a=b=c=d=e=f=g=h=i=j=0 ;
        cube_reference_point = Vector3D(0,0,0) ; 
        length=0 ; 
        width=0 ; 
        height=0 ; 
    }
    GeneralQuadSurface(double a , double b , double c , double d , double e , double f , double g , double h , double i , double j , Vector3D cube_reference_point , double length , double width , double height , Color color , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->a=a ; 
        this->b=b ; 
        this->c=c ; 
        this->d=d ; 
        this->e=e ; 
        this->f=f ; 
        this->g=g ; 
        this->h=h ; 
        this->i=i ; 
        this->j=j ; 
        this->cube_reference_point = cube_reference_point ; 
        this->length = length ; 
        this->width = width ; 
        this->height = height ; 
        this->color.setcolor(color) ;
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 
    }
    void showGeneralSurface()
    {
        cout<<"General Quadric Surface Information"<<endl ; 
        cout<<"a: "<<a<<" b: "<<b<<" c: "<<c<<" d: "<<d<<" e: "<<e<<" f: "<<f<<" g: "<<g<<" h: "<<h<<" i: "<<i<<" j: "<<j<<endl ;
        cout<<"Cube Reference Point: "<<cube_reference_point.x<<","<<cube_reference_point.y<<","<<cube_reference_point.z<<endl ; 
        cout<<"Length: "<<length<<endl ; 
        cout<<"Width: "<<width<<endl ; 
        cout<<"Height: "<<height<<endl ; 
        cout<<"Color: "<<color.r<<","<<color.g<<","<<color.b<<endl ;
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }
    friend istream &operator>>(istream &input , GeneralQuadSurface &gqs)
    {
        input>>gqs.a>>gqs.b>>gqs.c>>gqs.d>>gqs.e>>gqs.f>>gqs.g>>gqs.h>>gqs.i>>gqs.j>>gqs.cube_reference_point.x>>gqs.cube_reference_point.y>>gqs.cube_reference_point.z>>gqs.length>>gqs.width>>gqs.height>>gqs.color.r>>gqs.color.g>>gqs.color.b>>gqs.ambient>>gqs.diffuse>>gqs.specular>>gqs.reflection>>gqs.shine ; 
        return input ; 
    }
    void draw()override{

    }

    bool isinRange(Vector3D ip)
    {
        if(length!=0 && (ip.x<cube_reference_point.x || ip.x>cube_reference_point.x+length)) return false ;
        if(width!=0 && (ip.y<cube_reference_point.y || ip.y>cube_reference_point.y+width)) return false ;
        if(height!=0 && (ip.z<cube_reference_point.z || ip.z>cube_reference_point.z+height)) return false ;
        return true; 
    }
    double calculate_t(Ray &r , string str) override
    {
        //Ax2 + By2 + Cz2 + Dxy + Eyz + Fxz + Gx + Hy +Iz + J = 0 … …(1)
        //Px = R0x + t*Rdx, Similar for Py, Pz 
        //Put Px, Py, Pz as x, y, z in eq.(1) and solve for t
        //Accept the smaller non –ve real value of t
        // finally we will have a quad eqn in t , we will solve it and get the value of t 
        double a=0, b=0 , c=0 ; 
        a = this->a*r.direction.x*r.direction.x + this->b*r.direction.y*r.direction.y + this->c*r.direction.z*r.direction.z + this->d*r.direction.x*r.direction.y + this->e*r.direction.y*r.direction.z + this->f*r.direction.x*r.direction.z ;
        b = 2*this->a*r.start.x*r.direction.x + 2*this->b*r.start.y*r.direction.y + 2*this->c*r.start.z*r.direction.z + this->d*(r.start.x*r.direction.y + r.start.y*r.direction.x) + this->e*(r.start.y*r.direction.z + r.start.z*r.direction.y) + this->f*(r.start.x*r.direction.z + r.start.z*r.direction.x) + this->g*r.direction.x + this->h*r.direction.y + this->i*r.direction.z ;
        c = this->a*r.start.x*r.start.x + this->b*r.start.y*r.start.y + this->c*r.start.z*r.start.z + this->d*r.start.x*r.start.y + this->e*r.start.y*r.start.z + this->f*r.start.x*r.start.z + this->g*r.start.x + this->h*r.start.y + this->i*r.start.z + this->j ;
        double d = b*b - 4*a*c ;
        if (d<0) return -1 ;
        double t1 = (-b+sqrt(d))/(2*a) ;
        double t2 = (-b-sqrt(d))/(2*a) ;
        if(t1<0 && t2<0) return -1 ;
        Vector3D intersectionPoint1 = r.start.addition(r.direction.scalarMul(t1)) ;
        Vector3D intersectionPoint2 = r.start.addition(r.direction.scalarMul(t2)) ;
        if(t1>0 && isinRange(intersectionPoint1)) return t1 ; 
        if(t2>0 && isinRange(intersectionPoint2)) return t2 ;
        return -1 ; 
        

    }
    Vector3D getNormal(Vector3D intersectionPoint) override
    {
        double dx = 2*a*intersectionPoint.x + d*intersectionPoint.y + f*intersectionPoint.z + g ;
        double dy = 2*b*intersectionPoint.y + d*intersectionPoint.x + e*intersectionPoint.z + h ;
        double dz = 2*c*intersectionPoint.z + e*intersectionPoint.y + f*intersectionPoint.x + i ;
        Vector3D normal = Vector3D(dx,dy,dz) ; 
        normal.normalize() ; 
        return normal ; 
    }
    Color getColorAt(Vector3D intersectionPoint) override
    {
        Color xx; 
        xx.setcolor(color) ;
        return xx ;
    }

};


