#include <bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream> // header in standard library

#include "bitmap_image.hpp"
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2*acos(0.0))
#define epsilon .000001


using namespace std;

double determinantOfMatrix(double mat[3][3])
{
    double ans;
    ans = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
          - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
          + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return ans;
}



class Object;

class Vector3D
{
    public:
        double x;
        double y;
        double z;

        Vector3D();
        Vector3D(double x, double y, double z);
        Vector3D normalize();
        double dot(Vector3D b);
        Vector3D cross(Vector3D b);
        double distance(Vector3D b);
};

Vector3D::Vector3D()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D Vector3D::normalize()
{
    double length = sqrt(x*x+y*y+z*z);
    x = x/length;
    y = y/length;
    z = z/length;
    return *this;
}

double Vector3D::dot(Vector3D b)
{
    return x*b.x+y*b.y+z*b.z;
}

Vector3D Vector3D::cross(Vector3D b)
{
    Vector3D res;
    res.x = y*b.z - z*b.y;
    res.y = (-1)*(x*b.z - z*b.x);
    res.z = x*b.y - y*b.x;
    return res;
}

double Vector3D::distance(Vector3D b)
{
    return sqrt((x-b.x)*(x-b.x)+(y-b.y)*(y-b.y)+(z-b.z)*(z-b.z));
}

class PointLight{
    public:
        Vector3D light_pos;
        double color[3];
        void draw()
        {
            glPointSize(3);
            glBegin(GL_POINTS);
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(light_pos.x, light_pos.y, light_pos.z);
            glEnd();
        }
};

class SpotLight{
    public:
        PointLight point_light;
        Vector3D light_direction;
        double cutoff_angle;
        void draw()
        {
            glPointSize(7);
            glBegin(GL_POINTS);
            glColor3f(point_light.color[0], point_light.color[1], point_light.color[2]);
            glVertex3f(point_light.light_pos.x, point_light.light_pos.y, point_light.light_pos.z);
            glEnd();
        }
};


vector <Object*> objects;
vector <PointLight> pointLights;
vector <SpotLight> spotLights;
int recursionLevel;



class Ray
{
    public :
        Vector3D start;
        Vector3D dir;

        Ray(Vector3D start, Vector3D dir)
        {
            this->start=start;
            this->dir=dir.normalize();
        }

};

class Object {
        string type;
        double gen_coeffs[10];
    protected:
        double height, width, length;
        double color[3];
        double color_bmp[3];
        double coEfficients[4];	// ambient, diffuse, specular, reflection coefficients
        int shine; // exponent term of specular component
    public:
        Vector3D reference_point;	// should have x, y, z
        Object(){};
        Object(double gen_coeffs[10],Vector3D reference_point,double length,double width,double height);
        virtual void draw();
        void setColor(double color1, double color2, double color3);
        void setShine(int shine);
        void setCoEfficients(double ambient, double diffuse, double specular, double recursive_reflection_coefficient);
        double getLength();
        double* getColor();
        virtual double intersect(Ray *r, double *color, int level);
        void setType(string type);
        string getType();
        void setColorBmp(double* colors);
        double* getColorBmp();
        virtual void setDiffuseAndSpecular(Vector3D normal,Ray *r,Vector3D intersection_point,int level);

};

Object::Object(double *gen_coeffs,Vector3D reference_point,double length,double width,double height)
{
    for (int i= 0; i < 10; i++) {
        this->gen_coeffs[i]= gen_coeffs[i];
    }
    this->reference_point = reference_point;
    this->length = length;
    this->width = width;
    this->height = height;
    this->type="general";
}

void Object::setType(string type)
{
    this->type=type;
}

string Object::getType()
{
    return type;
}

void Object::setColor(double color1, double color2, double color3)
{
    this->color[0] = this->color_bmp[0] = color1;
    this->color[1] = this->color_bmp[1] = color2;
    this->color[2] = this->color_bmp[2] = color3;
}
void Object::setShine(int shine)
{
    this->shine = shine;
}
void Object::setCoEfficients(double ambient, double diffuse, double specular, double recursive_reflection_coefficient)
{
    this->coEfficients[0] = ambient;
    this->coEfficients[1] = diffuse;
    this->coEfficients[2] = specular;
    this->coEfficients[3] = recursive_reflection_coefficient;
}
double Object::getLength()
{
    return length;
}
double* Object::getColor()
{
    return color;
}

void Object::setColorBmp(double *colors)
{
    this->color_bmp[0] = colors[0];
    this->color_bmp[1] = colors[1];
    this->color_bmp[2] = colors[2];
}

double* Object::getColorBmp()
{
    return color_bmp;
}


void Object::draw()
{
//    for(int i=0;i<10;i++)
//    {
//        cout << gen_coeffs[i] << " ";
//    }
}

double Object::intersect(Ray *r, double *color, int level)
{
        Vector3D translated;
        translated.x = r->start.x-reference_point.x;
        translated.y = r->start.y-reference_point.y;
        translated.z = r->start.z-reference_point.z;

        double a = gen_coeffs[0]*r->dir.x*r->dir.x + gen_coeffs[1]*r->dir.y*r->dir.y + gen_coeffs[2]*r->dir.z*r->dir.z +
                    gen_coeffs[3]*r->dir.x*r->dir.y + gen_coeffs[4]*r->dir.y*r->dir.z + gen_coeffs[5]*r->dir.x*r->dir.z;
        double b = 2*gen_coeffs[0]*translated.x*r->dir.x + 2*gen_coeffs[1]*translated.y*r->dir.y + 2*gen_coeffs[2]*translated.z*r->dir.z
                    + gen_coeffs[3]*(r->dir.x*translated.y+translated.x*r->dir.y) + gen_coeffs[4]*(translated.y*r->dir.z+translated.z*r->dir.y)
                    + gen_coeffs[5]*(r->dir.z*translated.x+translated.z*r->dir.x) + + gen_coeffs[6]*r->dir.x + gen_coeffs[7]*r->dir.y +
                    gen_coeffs[8]*r->dir.z;
        double c = gen_coeffs[0]*translated.x*translated.x + gen_coeffs[1]*translated.y*translated.y + gen_coeffs[2]*translated.z*translated.z
                    + gen_coeffs[3]*translated.x*translated.y + gen_coeffs[4]*translated.y*translated.z + gen_coeffs[5]*translated.x*translated.z
                    + gen_coeffs[6]*translated.x + gen_coeffs[7]*translated.y + gen_coeffs[8]*translated.z + gen_coeffs[9];

        double d=sqrt(b*b-4*a*c);
        if(d<0)
            return -1;

        double t1=(-b-d)/(2*a);
        double t2=(-b+d)/(2*a);



        Vector3D intersection_point1;
        intersection_point1.x = r->start.x + t1*r->dir.x;
        intersection_point1.y = r->start.y + t1*r->dir.y;
        intersection_point1.z = r->start.z + t1*r->dir.z;

        Vector3D intersection_point2;
        intersection_point2.x = r->start.x + t2*r->dir.x;
        intersection_point2.y = r->start.y + t2*r->dir.y;
        intersection_point2.z = r->start.z + t2*r->dir.z;

        if(length!=0.0)
        {
            if(intersection_point1.x < reference_point.x || intersection_point1.x > reference_point.x+length )
                t1 = -1;
            if(intersection_point2.x < reference_point.x || intersection_point2.x > reference_point.x+length )
                t2 = -1;
        }

        if(width!=0.0)
        {
            if(intersection_point1.y < reference_point.y || intersection_point1.y > reference_point.y+width)
                t1 = -1;
            if(intersection_point2.y < reference_point.y || intersection_point2.y > reference_point.y+width)
                t2 = -1;
        }

        if(height!=0.0)
        {
            if(intersection_point1.z < reference_point.z || intersection_point1.z > reference_point.z+height)
                t1 = -1;
            if(intersection_point2.z < reference_point.z || intersection_point2.z > reference_point.z+height)
                t2 = -1;
        }

        double t;
        if(t1>0) t=t1;
        else t=t2;
        if(level==0)
            return t;

        Vector3D intersection_point,normal;

        intersection_point.x = r->start.x + t*r->dir.x;
        intersection_point.y = r->start.y + t*r->dir.y;
        intersection_point.z = r->start.z + t*r->dir.z;

        normal.x = 2*gen_coeffs[0]*intersection_point.x + gen_coeffs[3]*intersection_point.y + gen_coeffs[4]*intersection_point.z + gen_coeffs[6];
        normal.y = 2*gen_coeffs[1]*intersection_point.y + gen_coeffs[3]*intersection_point.x + gen_coeffs[5]*intersection_point.z + gen_coeffs[7];
        normal.z = 2*gen_coeffs[2]*intersection_point.z + gen_coeffs[4]*intersection_point.x + gen_coeffs[5]*intersection_point.y + gen_coeffs[8];

        normal = normal.normalize();

        //check position
    //    if(r->start.distance(reference_point)<length)
    //    {
    //        normal.x*=-1;
    //        normal.y*=-1;
    //        normal.z*=-1;
    //    }

        //diffuse and specular
        setDiffuseAndSpecular(normal,r,intersection_point,level);

        return t;
}

void Object::setDiffuseAndSpecular(Vector3D normal,Ray *r,Vector3D intersection_point,int level)
{
    double *color_final,*color_initial;
    color_initial=getColor();
    color_final = new double[3];
    for(int i=0;i<3;i++)
    {
        color_final[i] = color_initial[i]*coEfficients[0];
    }

    vector<PointLight> tempLights=pointLights;

    for (auto & light : spotLights)
    {
        light.light_direction = light.light_direction.normalize();
        Vector3D dir;
        dir.x = intersection_point.x-light.point_light.light_pos.x;
        dir.y = intersection_point.y-light.point_light.light_pos.y;
        dir.z = intersection_point.z-light.point_light.light_pos.z;

        dir = dir.normalize();
//        cout << "reaches 1" << endl;

        double angle = acos(light.light_direction.dot(dir));
        angle*=(180/pi);
        if(angle<=light.cutoff_angle)
        {
            tempLights.push_back(light.point_light);
        }

    }

    for (auto & light : tempLights)
    {
        Vector3D ir_dir;
        ir_dir.x = intersection_point.x-light.light_pos.x;
        ir_dir.y = intersection_point.y-light.light_pos.y;
        ir_dir.z = intersection_point.z-light.light_pos.z;

        Ray *ir = new Ray(light.light_pos,ir_dir);

//        cout << "reaches 1" << endl;
        if(type=="floor")
        {
            if(ir->start.z<=0)
                normal.z=-1;
        }


        Vector3D rr_dir;
        double l_dot_n = ir->dir.dot(normal);
        rr_dir.x = ir->dir.x - 2*l_dot_n*normal.x;
        rr_dir.y = ir->dir.y - 2*l_dot_n*normal.y;
        rr_dir.z = ir->dir.z - 2*l_dot_n*normal.z;


        Ray *rr = new Ray(intersection_point,rr_dir);
        double *dummycolor;
//        double *dummycolor = new double[3];
//
////        cout << "reaches 2" << endl;
//
//        for(int i=0;i<3;i++)
//        {
//            dummycolor[i]=0;
//        }

        double dist = light.light_pos.distance(intersection_point);
        double t,tMin = INT_MAX;

        for (auto & o : objects)
        {
            //for each object, o in objects
            t = o->intersect(ir, dummycolor, 0);
            if(t<(dist-epsilon) && t>0)
            {
                tMin=t;
                break;
            }
        }

        if(tMin!=INT_MAX) continue;

//        cout << "reaches 4" << endl;

        double lambert_value = max(0.0,-(ir->dir.dot(normal)));
        double phong_value = pow(max(0.0,-(r->dir.dot(rr->dir))),shine);
        cout<<lambert_value<<" "<<phong_value<<endl;
        


        for(int i=0;i<3;i++)
        {
            color_final[i] += light.color[i]*(coEfficients[1]*lambert_value + coEfficients[2]*phong_value)*color_initial[i];
            if(color_final[i]>1) color_final[i]=1;
            if(color_final[i]<0) color_final[i]=0;
        }
    }
    cout<<color_final[0]<<" "<<color_final[1]<<" "<<color_final[2]<<endl;
    setColorBmp(color_final);

    if(level>=recursionLevel)
        return;

    Vector3D rr_dir;

    if(type=="floor")
    {
        if(r->start.z<=0)
            normal.z=-1;
    }

    double l_dot_n = r->dir.dot(normal);
    rr_dir.x = r->dir.x - 2*l_dot_n*normal.x;
    rr_dir.y = r->dir.y - 2*l_dot_n*normal.y;
    rr_dir.z = r->dir.z - 2*l_dot_n*normal.z;


    Vector3D rr_start;

    rr_start.x = intersection_point.x + rr_dir.x*epsilon;
    rr_start.y = intersection_point.y + rr_dir.y*epsilon;
    rr_start.z = intersection_point.z + rr_dir.z*epsilon;

    Ray *rr = new Ray(rr_start,rr_dir);


    double *dummycolor;
//    double *dummycolor = new double[3];
//
////        cout << "reaches 2" << endl;
//
//    for(int i=0;i<3;i++)
//    {
//        dummycolor[i]=0;
//    }

    double t,tMin = INT_MAX;
    int nearest=-1;
    int idx=0;
    double* color_reflected;

    for (auto & o : objects)
    {
        //for each object, o in objects
        t = o->intersect(rr, dummycolor, 0);
        if(t<tMin && t>0)
        {
            tMin=t;
            nearest=idx;
        }
        idx++;
    }
    if(tMin!=INT_MAX)
    {
        Object* n = objects[nearest];
        tMin = n->intersect(rr,color_reflected,level+1);
        color_reflected = n->getColorBmp();
        for(int i=0;i<3;i++)
        {
            color_final[i] += color_reflected[i]*coEfficients[3];
            if(color_final[i]>1) color_final[i]=1;
            if(color_final[i]<0) color_final[i]=0;
        }
    }
    setColorBmp(color_final);

}


class Sphere : public Object {
    public:
        Sphere(Vector3D center, double radius)
        {
            reference_point = center;
            length = radius;
        }
        void draw();
        double intersect(Ray* r,double* color,int level);
};

void Sphere::draw()
{
//    cout << this->reference_point.x << " " << this->reference_point.y << " " << this->reference_point.z << endl;
//    cout << length << endl;
    double radius=length;
    glColor3f(color[0],color[1],color[2]);
	Vector3D points[100][100];
	int slices=24;
	int stacks=30;
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
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

//double Sphere::intersect(Ray* r,double* color,int level)
//{
//    color = getColor();
//    Vector3D translated;
//    translated.x = r->start.x-reference_point.x;
//    translated.y = r->start.y-reference_point.y;
//    translated.z = r->start.z-reference_point.z;
//
//    double ld = -translated.dot(r->dir);
//    if(ld<0) return -1.0;
//    double d_squared = translated.dot(r->start) - ld*ld;
//    if(d_squared>length*length) return -1.0;
//    double l_squared=length*length - d_squared;
//    double t1 = ld+sqrt(l_squared);
//    double t2 = ld-sqrt(l_squared);
//    return t1>t2 ? t2 : t1;
//}


double Sphere::intersect(Ray* r,double* clr,int level)
{
    //start-center
    Vector3D translated;
    translated.x = r->start.x-reference_point.x;
    translated.y = r->start.y-reference_point.y;
    translated.z = r->start.z-reference_point.z;
    double a=1;
    double b=2*translated.dot(r->dir);
    double c=translated.dot(translated)-length*length;

    double d=sqrt(b*b-4*a*c);

    double t1=(-b-d)/(2*a);
    double t2=(-b+d)/(2*a);

    double t;
    if(d<0)
        return -1;
    else
    {
        if(t1>0) t = t1;
        else t = t2;
    }
    // if(t>0)
    // {
    //         cout<<r->start.x<<" "<<r->start.y<<" "<<r->start.z<<endl;
    // cout<<r->dir.x<<" "<<r->dir.y<<" "<<r->dir.z<<endl;
    // cout<<a<<" "<<b<<" "<<c<<" "<<d<<endl ;  cout<<"t"<<t<<endl ; 
    // }
   
    
    if(level==0)
        return t;
//    clr = getColor();
    setColorBmp(getColor());


    Vector3D intersection_point,normal;

    intersection_point.x = r->start.x + t*r->dir.x;
    intersection_point.y = r->start.y + t*r->dir.y;
    intersection_point.z = r->start.z + t*r->dir.z;

    normal.x = intersection_point.x - reference_point.x;
    normal.y = intersection_point.y - reference_point.y;
    normal.z = intersection_point.z - reference_point.z;

    normal = normal.normalize();

    //check position
//    if(r->start.distance(reference_point)<length)
//    {
//        normal.x*=-1;
//        normal.y*=-1;
//        normal.z*=-1;
//    }

    //diffuse and specular
    setDiffuseAndSpecular(normal,r,intersection_point,level);



    return t;

}

class Triangle : public Object {

    Vector3D point1;
    Vector3D point2;
    Vector3D point3;

    public:
        Triangle(Vector3D point1,Vector3D point2,Vector3D point3)
        {
            reference_point.x = reference_point.y = reference_point.z = 0;
            this->point1 = point1;
            this->point2 = point2;
            this->point3 = point3;
        }
        void draw();
        double intersect(Ray* r,double* color,int level);
};

void Triangle::draw()
{
//    cout << this->point1.x << " " << this->point1.y << " " << this->point1.z << endl;
//    glTranslatef(this->reference_point.x,this->reference_point.y,this->reference_point.z);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(point1.x,point1.y,point1.z);
        glVertex3f(point2.x,point2.y,point2.z);
        glVertex3f(point3.x,point3.y,point3.z);
    }
    glEnd();
}

double Triangle::intersect(Ray* r,double* color,int level)
{
    double A[3][3];

    A[0][0] = point1.x-point2.x;
    A[1][0] = point1.y-point2.y;
    A[2][0] = point1.z-point2.z;

    A[0][1] = point1.x-point3.x;
    A[1][1] = point1.y-point3.y;
    A[2][1] = point1.z-point3.z;

    A[0][2] = r->dir.x;
    A[1][2] = r->dir.y;
    A[2][2] = r->dir.z;



    double detA = determinantOfMatrix(A);

    if(round(detA)==0)
        return -1;


//    cout << "detA " << detA << endl;

    double betaMatrix[3][3];
    std::copy(&A[0][0], &A[0][0] + 3 * 3, &betaMatrix[0][0]);


    betaMatrix[0][0] = point1.x-r->start.x;
    betaMatrix[1][0] = point1.y-r->start.y;
    betaMatrix[2][0] = point1.z-r->start.z;


    double detBeta = determinantOfMatrix(betaMatrix);
    double beta = detBeta/detA;

//    cout << "beta " << beta << endl;

    if(beta<=0) return -1;

    double gammaMatrix[3][3];
    std::copy(&A[0][0], &A[0][0] + 3 * 3, &gammaMatrix[0][0]);
    gammaMatrix[0][1] = point1.x-r->start.x;
    gammaMatrix[1][1] = point1.y-r->start.y;
    gammaMatrix[2][1] = point1.z-r->start.z;

    double detGamma = determinantOfMatrix(gammaMatrix);
    double gamma = detGamma/detA;

//    if(gamma>0)
//    cout << "gamma " << gamma << endl;

    if(gamma<=0) return -1;

//    if(beta+gamma<1)
//    cout << "beta+gamma " << beta+gamma << endl;

    if(beta+gamma>=1) return -1;

    double tMatrix[3][3];
    std::copy(&A[0][0], &A[0][0] + 3 * 3, &tMatrix[0][0]);
    tMatrix[0][2] = point1.x-r->start.x;
    tMatrix[1][2] = point1.y-r->start.y;
    tMatrix[2][2] = point1.z-r->start.z;

    double detT = determinantOfMatrix(tMatrix);
    double t = detT/detA;

//    cout << "t " << t << endl;

    if(t<=0) return -1;
    if(level==0)
        return t;

    Vector3D intersection_point,normal;

    intersection_point.x = r->start.x + t*r->dir.x;
    intersection_point.y = r->start.y + t*r->dir.y;
    intersection_point.z = r->start.z + t*r->dir.z;

    Vector3D side1,side2;
    side1.x = point2.x-point1.x;
    side1.y = point2.y-point1.y;
    side1.z = point2.z-point1.z;

    side2.x = point3.x-point1.x;
    side2.y = point3.y-point1.y;
    side2.z = point3.z-point1.z;

    normal = side1.cross(side2);

    normal = normal.normalize();

    //check position
//    if(r->start.distance(reference_point)<length)
//    {
//        normal.x*=-1;
//        normal.y*=-1;
//        normal.z*=-1;
//    }

    //diffuse and specular
    setDiffuseAndSpecular(normal,r,intersection_point,level);



    return t;

}

class Floor : public Object {
    int floorWidth;
    int tileWidth;
    public:
        Floor(double floorWidth, double tileWidth)
        {
            this->floorWidth = floorWidth;
            this->tileWidth = tileWidth;
            reference_point.x = -floorWidth/2;
            reference_point.y = -floorWidth/2;
            reference_point.z = 0;
            length = tileWidth;
            this->setType("floor");
            setCoEfficients(0.4, 0.2, 0.2, 0.2);
            shine=5;

        }
        void draw();
        double intersect(Ray* r,double* color,int level);
};


void Floor::draw()
{
    glPushMatrix();
//    glTranslatef(this->reference_point.x,this->reference_point.y,this->reference_point.z);
    	//grey
    int row = (int) (floorWidth/tileWidth);
    int column = row;

    double init_x = reference_point.x;
    double init_y = reference_point.y;

    int color=0;

    for(int i=0;i<row;i++)
    {
        color=1-color;
        glColor3f(color,color,color);
        init_x=i*tileWidth;
        for(int j=0;j<column;j++)
        {
            color=1-color;
            glColor3f(color,color,color);

            init_y=j*tileWidth;
            glBegin(GL_QUADS);{

            //lines parallel to Y-axis
            glVertex3f(init_x,init_y,0);
            glVertex3f(init_x+tileWidth,init_y,0);

            //lines parallel to X-axis
            glVertex3f(init_x+tileWidth,init_y+tileWidth,0);
            glVertex3f(init_x,init_y+tileWidth,0);
            }glEnd();
        }
    }
    glPopMatrix();


}

double Floor::intersect(Ray* r,double* color,int level)
{
    Vector3D p0 = reference_point;
    Vector3D normal;
    normal.x = 0;
    normal.y = 0;
    normal.z = 1;



    double denom = r->dir.dot(normal);
    if(denom==0)
        return -1;
    double nom = p0.dot(normal) - r->start.dot(normal);
//
    double t=nom/denom;
//
    Vector3D intersection_point;
    intersection_point.x = r->start.x + t*r->dir.x;
    intersection_point.y = r->start.y + t*r->dir.y;
    intersection_point.z = r->start.z + t*r->dir.z;

    if(intersection_point.x<-floorWidth/2 || intersection_point.x>floorWidth/2 || intersection_point.y<-floorWidth/2 || intersection_point.y>floorWidth/2)
        return -1;
//
//
    int row_index = (intersection_point.x-reference_point.x)/tileWidth;
    int col_index = (intersection_point.y-reference_point.y)/tileWidth;

    if(row_index%2==0)
    {
        if(col_index%2==0)
        {
            setColor(1,1,1);
        }
        else
        {
            setColor(0,0,0);
        }
    }
    else
    {
        if(col_index%2==0)
        {
            setColor(0,0,0);
        }
        else
        {
            setColor(1,1,1);
        }
    }
//
//
    if(level==0)
        return t;
//
//
//    //check position
////    if(r->start.distance(reference_point)<length)
////    {
////        normal.x*=-1;
////        normal.y*=-1;
////        normal.z*=-1;
////    }
//
//    //diffuse and specular
    setDiffuseAndSpecular(normal,r,intersection_point,level);
//
//
    return t;
}








//int main()
//{
//    Object *obj;
//    Vector3D *v = new Vector3D(1,2,3);
//    Sphere *sph = new Sphere(*v,5);
//    obj = sph;
//    obj->draw();
//}
