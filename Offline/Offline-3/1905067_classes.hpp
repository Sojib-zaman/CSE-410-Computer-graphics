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

vector<double> color_norm(vector<double>color)
{
    // if color goes beyond 1 then we will make it 1
    // if color goes below 0 then we will make it 0
    vector<double> newColor ;
    newColor.push_back(max(0.0,min(1.0,color[0]))) ;
    newColor.push_back(max(0.0,min(1.0,color[1]))) ;
    newColor.push_back(max(0.0,min(1.0,color[2]))) ;
    return newColor ;
}


vector<double> color_multiplication(vector<double>color,  double value)
{
    vector<double> newColor ; 
    newColor.push_back(color[0]*value) ; 
    newColor.push_back(color[1]*value) ; 
    newColor.push_back(color[2]*value) ; 
    return color_norm(newColor) ; 
}
vector<double> color_addition(vector<double>color1 , vector<double>color2)
{
    vector<double> newColor ; 
    newColor.push_back(color1[0]+color2[0]) ; 
    newColor.push_back(color1[1]+color2[1]) ; 
    newColor.push_back(color1[2]+color2[2]) ; 
    return color_norm(newColor) ; 
}

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
void drawSphere(double radius , double color[] ,int slices , int stacks)
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
                    glColor3f(color[0],color[1],color[2]) ; 
                    
               
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
class Light
{
    public: 
    Vector3D Super_position ; 
};
class PointLight : public Light{
    public:
    Vector3D light_position;
    double color[3];
    PointLight() {
        
        light_position = Vector3D(0, 0, 0);
        Super_position = light_position ; 
    }
    PointLight(Vector3D poisition , double color[3])
    {
        this->light_position = poisition ; 
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
        Super_position = light_position ;

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
    void draw()
    {
        glPushMatrix() ; 
        glTranslatef(light_position.x,light_position.y,light_position.z) ;
        drawSphere(4,color,24,24) ;
        glPopMatrix() ;
    }

};
class SpotLight : public Light{
    public:
    PointLight point_light ;
    Vector3D light_direction ;
    double cutoff_angle ; 
    SpotLight()
    {
        point_light = PointLight() ; 
        light_direction = Vector3D(0,0,0) ; 
        cutoff_angle = 0 ; 
        Super_position = point_light.light_position ;
    
    }
    SpotLight(PointLight pl , Vector3D ld , double c_angle)
    {
        point_light = pl  ; 
        light_direction = ld ;
        cutoff_angle = c_angle ;
        light_direction.normalize() ; 
        Super_position = point_light.light_position ;
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
        drawSphere(4,point_light.color,24,24) ;
        glPopMatrix() ;
    }

};

void showVecCol(vector<double>col)
{
    //cout<<"color "<<col[0]<<" "<<col[1]<<" "<<col[2]<<endl ; 
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
    string type ;

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
    virtual vector<double> getColorAt(Vector3D intersectionPoint)
    {
        vector<double> xx = {color[0],color[1],color[2]} ; 
        return xx ;
    }
    virtual Vector3D getNormal(Vector3D IntersectionPoint)
    {
        return Vector3D(0,0,0) ;
    }

    //following the intersect pseudo code given on spec
double intersect(Ray &r , double current_color[3] , int level)
    {
        
        double t = this->calculate_t(r , this->type) ; 
        //cout<<t<<endl ; 
        if(level==0) return t ; 




        // so if level is not 0 then we have to calculate the color of the object
        // now at the intersection point , we calculate the color based on Phong model (ambient,diffuse,specular)
        Vector3D intersectionPoint = findInterSectionPoint(r,t) ;
        vector<double> intersectionCol;
        vector<double> color; 
        intersectionCol = getColorAt(intersectionPoint) ;
        color = color_multiplication(intersectionCol,ambient) ;
        Vector3D normal = getNormal(intersectionPoint) ;

       



        //cout<<"starting lights"<<endl ; 
        vector<Light> all_lights ;
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
            if(convert_to_degree(createdAngle) <=spot_lights[i].cutoff_angle) all_lights.push_back(spot_lights[i]) ; 
        
        }
       // cout<<"spot light done"<<endl ; 
        // now all lights contain both the pointlight and the spotlights (that are valid)
        for(int i=0 ; i<all_lights.size() ; i++)
        {
            // Id = Ip . kd . (L.N)
            // Is = Ip . ks . (R.V)^K 
            // so have to find L and R first , N is in the normal 
            Vector3D L , R , V , N ;
            L = all_lights[i].Super_position.subtraction(intersectionPoint) ;
            N = normal ;  
            R = N.scalarMul(2*(L.dotMul(N))).subtraction(L) ;
            V = position_of_camera.subtraction(intersectionPoint) ;


            L.normalize() ;
            R.normalize() ;
            N.normalize() ;
            V.normalize() ;
            double LdotN = L.dotMul(N) ;
            double RdotV = R.dotMul(V) ;
            
            //sending light from light source to the intersection point 
            Ray incidentRay(all_lights[i].Super_position,L) ; 
            //the ray from intersection point to reflection direction 
            Ray reflectedRay(intersectionPoint,R) ;

            // now we have to check if the incident ray intersects with any object or not before reaching the intersection point
            // if it intersects then we will not consider the light source
            bool isBlocked = false ;
            double t2, min_t = 1000000 ;
            for(Object *o : objects)
            {
                t2 = o->calculate_t(incidentRay , "line 441") ; //basically calling the intersect function of the object , with level 0 it will go to calculate_t
                if(t2>0 && t2<min_t)
                {
                    min_t = t2 ;
                }
            }
            //cout<<"blocked calculation done"<<endl ; 
            if(min_t<t) isBlocked = true ;

            if(!isBlocked)
            {
                vector<double> Id = color_multiplication(intersectionCol,diffuse*LdotN) ;
                vector<double> Is = color_multiplication(intersectionCol,specular*pow(RdotV,shine)) ;
                color = color_addition(color,Id) ;
                color = color_addition(color,Is) ;
            }
      
        

        }
        //cout<<recursion_level<<endl ; 
        if(level==recursion_level) return t ;
        //set up the current color 
        current_color[0] = color[0] ;
        current_color[1] = color[1] ;
        current_color[2] = color[2] ;


        //showVecCol(color) ; 



        return t ; 
    }
    
    
    virtual double calculate_t(Ray &r , string str)
    {
            //cout<<"This is the base class"<<str<<endl ;
            return 100 ; 
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
    Floor(Vector3D initial_point ,double floorwidth ,  double tilewidth , double color[3] , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->initial_point = initial_point ;
        this->reference_point = Vector3D(0,0,0) ;
        this->floorwidth = floorwidth ;
        this->tilewidth = tilewidth ; 
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
        this->ambient=ambient ; 
        this->diffuse=diffuse ; 
        this->specular=specular ; 
        this->reflection=reflection ; 
        this->shine=shine ; 
    }
    void draw()
    {
        int floorRow = (int)(floorwidth/tilewidth) ;
        int floorCol = (int)(floorwidth/tilewidth) ; 
        for(int i=0 ; i<floorRow ; i++)
        {
            for(int j=0 ; j<floorCol ; j++)
            {
                if((i+j)%2==0)
                {
                    //setcolor(white) ; 
                    glColor3f(color[0],color[1],color[2]) ;
                }
                else
                {
                    //setcolor(black) ; 
                    glColor3f(1-color[0],1-color[1],1-color[2]) ; 
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
        cout<<"Color: "<<color[0]<<","<<color[1]<<","<<color[2]<<endl ; 
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    }
    double calculate_t(Ray &r , string str) override
    {

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
    Sphere(Vector3D center, double radius , double color[3])
    {
        this->reference_point = center ; 
        this->center = center;
        this->radius = radius;
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
    }
    Sphere(Vector3D center, double radius , double color[3] , double ambient , double diffuse , double specular ,double reflection ,  int shine)
    {
        this->reference_point = center ; 
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
    Vector3D getNormal(Vector3D intersectionPoint)
    {
        Vector3D normal = intersectionPoint.subtraction(center) ; 
        normal.normalize() ; 
        return normal ; 
    }

    void draw() {
  
       int slices = 24 ; 
       int stacks = 24 ; 
       glPushMatrix() ; 
       drawSphere(radius , color ,slices , stacks) ; 
       glPopMatrix() ; 
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
    vector<double> getColorAt(Vector3D intersectionPoint)
    {
        
        vector<double> xx = {color[0],color[1],color[2]}; 
    
        return xx; 
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
        this->reference_point = Vector3D(0,0,0) ;
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
    void draw()
    {
        glBegin(GL_TRIANGLES) ; 
        glColor3f(color[0],color[1],color[2]) ; 
        glVertex3f(p1.x,p1.y,p1.z) ; 
        glVertex3f(p2.x,p2.y,p2.z) ; 
        glVertex3f(p3.x,p3.y,p3.z) ; 
        glEnd() ; 
    
    }
    double calculate_t(Ray &r , string str) override
    {

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
    GeneralQuadSurface(double a , double b , double c , double d , double e , double f , double g , double h , double i , double j , Vector3D cube_reference_point , double length , double width , double height , double color[3] , double ambient , double diffuse , double specular ,double reflection ,  int shine)
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
        this->color[0]=color[0] ; 
        this->color[1]=color[1] ; 
        this->color[2]=color[2] ; 
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
        cout<<"Color: "<<color[0]<<","<<color[1]<<","<<color[2]<<endl ; 
        cout<<"Ambient: "<<ambient<<endl ; 
        cout<<"Diffuse: "<<diffuse<<endl ; 
        cout<<"Specular: "<<specular<<endl ; 
        cout<<"Reflection: "<<reflection<<endl ; 
        cout<<"Shine: "<<shine<<endl ; 
    
    }
    friend istream &operator>>(istream &input , GeneralQuadSurface &gqs)
    {
        input>>gqs.a>>gqs.b>>gqs.c>>gqs.d>>gqs.e>>gqs.f>>gqs.g>>gqs.h>>gqs.i>>gqs.j>>gqs.cube_reference_point.x>>gqs.cube_reference_point.y>>gqs.cube_reference_point.z>>gqs.length>>gqs.width>>gqs.height>>gqs.color[0]>>gqs.color[1]>>gqs.color[2]>>gqs.ambient>>gqs.diffuse>>gqs.specular>>gqs.reflection>>gqs.shine ; 
        return input ; 
    }
    double calculate_t(Ray &r , string str) override
    {

    }

};


