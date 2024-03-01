#include <bits/stdc++.h>
using namespace std ; 
#include <ctime>
#include <cstdlib>
#include <chrono>
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>
#include <chrono>
#include "1905067_classes.hpp"
#include "bitmap_image.hpp"
//+ VARIABLES CAN BE FOUND HERE  -----------------------------------------------------------

//. Input variables
int recursion_level ;
int number_of_pixels ;
int number_of_objects ;
int number_of_pointLights ; 
int number_of_spotLights ;

//. Objects and lights 
vector<Object*> objects ;
vector<PointLight> point_lights ;
vector<SpotLight> spot_lights ;

//. Floor variables
double floorwidth = 1000.0 ;
double tile_width = 20.0 ;   


//. Camera variables
double cameraGlutAngle = 80.0 ;

//. Image variables
int imageCount = 1 ; 
//+ ----------------------------------------------------------------------------------------
double angle_converter(double degree) {
    return degree * M_PI / 180.0;
}


void capture()
{
    ofstream fout ;
    fout.open("output.txt") ;

    bitmap_image bitImage = bitmap_image(number_of_pixels , number_of_pixels) ;
    for(int column=0 ; column<number_of_pixels ; column++)
    {
        for(int row=0 ; row<number_of_pixels ; row++)
            bitImage.set_pixel(column,row,0,0,0) ; 
    }
    string imageName = "Output_1"+to_string(imageCount)+".bmp" ;
    imageCount++ ;

    double window_width = 500.0 ;
    double window_height = 500.0 ;
    double viewAngle = 80.0 ;
    double plainDistance = (window_height/2.0)/tan(angle_converter(viewAngle)/2.0) ;
    Vector3D topLeft = position_of_camera.addition(camera_look.scalarMul(plainDistance)).subtraction(camera_right.scalarMul(window_width/2.0)).addition(camera_up.scalarMul(window_height/2.0)) ;
    double du = window_width/number_of_pixels ;
    double dv = window_height/number_of_pixels ;
    topLeft = topLeft.addition(camera_right.scalarMul(du/2.0)).subtraction(camera_up.scalarMul(dv/2.0)) ;
    
    int nearest = -4 ; 
   
    
    for(int i=0 ; i<number_of_pixels ; i++)
    {
        for(int j=0 ; j<number_of_pixels ; j++)
        {
            Color color ;
            Vector3D current_pixel = topLeft.addition(camera_right.scalarMul(i*du)).subtraction(camera_up.scalarMul(j*dv)) ;
            Ray ray = Ray(position_of_camera , current_pixel.subtraction(position_of_camera)) ;
            double tMin = 1000000000.0 ; // a large number for tmin
            int obj_count = 0 ; 


           
            for(Object *o : objects)
            {
                double t = o->intersect(ray , color , 0) ;
                // 0 means that color is not needed
                if(t<tMin && t>0)
                {
                    tMin = t ; 
                    nearest = obj_count ; 
                    fout<<"in main"<<tMin<<endl ;  
                }
                obj_count++ ;
            }
            if(nearest!=-4)
            {
                Object *nearest_Object; 
                nearest_Object= objects[nearest] ; 
                tMin = nearest_Object->intersect(ray , color , 1) ; // 1 means that color is needed and will be computed
                //color.show() ;
                color.normalize() ; 
                color.show() ; 
        
                bitImage.set_pixel(i,j,color.r*255,color.g*255,color.b*255) ; 
            }
            //nearest object er color diye color kora lagbe
           
           
            nearest = -4;
            tMin = 1000000000.0 ; // a large number for tmin

        }
    }
    bitImage.save_image(imageName) ;
    cout<<"Image saved as "<<imageName<<endl ;


    
}
void showVector3DInfo()
{
    camera_up.show("camera_up") ;
    camera_look.show("camera_look") ;
    camera_right.show("camera_right") ;
    position_of_camera.show("position_of_camera") ;
}
void keyboard(unsigned char key , int a , int b)
{
    //showVector3DInfo() ; 
    switch (key)
    {
    case '2':
        camera_look = RodriGeneral(camera_look , camera_up , 0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , 0.1) ; 
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
    case '0':
        capture() ; 
        break;
    case ' ':
        exit(0) ;
        break;
    default:
        break;
    }
}



void keystrokehandler(int key , int x  , int y)
{
   // showVector3DInfo() ;
    switch (key)
    {
    case GLUT_KEY_UP:
        position_of_camera=position_of_camera.addition(camera_look.scalarMul(4.0)) ; 
        break;
    case GLUT_KEY_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_look.scalarMul(4.0)) ; 
        break;
    case GLUT_KEY_LEFT:
        position_of_camera=position_of_camera.addition(camera_right.scalarMul(4.0)) ; 
        break;
    case GLUT_KEY_RIGHT:
        position_of_camera=position_of_camera.subtraction(camera_right.scalarMul(4.0)) ; 
        break;
    case GLUT_KEY_PAGE_UP:
        position_of_camera=position_of_camera.addition(camera_up.scalarMul(4.0)) ; 
        break;
    case GLUT_KEY_PAGE_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_up.scalarMul(4.0)) ; 
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
    gluPerspective(cameraGlutAngle,1,1,1000); 
    
    position_of_camera = Vector3D(202.405,210.518,100.642) ; 
    camera_look = Vector3D(-0.465755,-0.547041,-0.695571) ;
    camera_right = Vector3D(0.781163,-0.623468,-0.0327333) ; 
    camera_up = Vector3D(-0.41576,-0.5586,0.717711) ; 


    position_of_camera = Vector3D(-127.133,139.557,167.81) ; 
    camera_look = Vector3D(0.602433,-0.683076,-0.412892) ;
    camera_right = Vector3D(0.709247,0.695411,-0.115635) ;
    camera_up = Vector3D(0.366117,-0.223181,0.903409) ;


    position_of_camera = Vector3D(100,100,50) ;
    camera_look = Vector3D(-0.707, -0.707, 0) ;
    camera_right = Vector3D(-0.707, 0.707, 0) ;
    camera_up = Vector3D(0, 0, 1) ;


    

}
void drawElements()
{
    for(Object *o : objects)
    {
        glPushMatrix() ; 
        glTranslatef(o->reference_point.x , o->reference_point.y , o->reference_point.z) ;
        o->draw() ;
        glPopMatrix() ;  
    }
    for(PointLight pl : point_lights)
    {
        pl.draw() ; 
    }
    for(SpotLight sl : spot_lights)
    {
        sl.draw() ; 
    }
}

// display function 
void RayTracing()
{


    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); // Now working on model view matrix 
    glLoadIdentity(); // all previous matrix cleared 

    gluLookAt(position_of_camera.getx() , position_of_camera.gety() , position_of_camera.getz() , 
    position_of_camera.getx()+camera_look.getx() , position_of_camera.gety()+camera_look.gety() , position_of_camera.getz()+camera_look.getz() , 
    camera_up.getx() , camera_up.gety() , camera_up.getz()
    ) ; 

   glPushMatrix() ;  
   drawElements() ;
   glPopMatrix() ; 

    

    glutSwapBuffers() ; 
}
void idle()
{    
    glutPostRedisplay(); 
}
void timer(int val)
{

}
void pushFloor()
{
    Vector3D start = Vector3D(-floorwidth/2,-floorwidth/2,0);  
    Color color = Color(0,0,0) ; 
    Floor floor (start , floorwidth, tile_width , color, .8,.8,.8,.8,1) ;
    Object *temp ; 
    temp = new Floor(floor.initial_point , floor.floorwidth , floor.tilewidth , floor.color , floor.ambient , floor.diffuse , floor.specular , floor.reflection , floor.shine) ;
    objects.push_back(temp) ;
    //floor.showFloorInformation() ; 
   

}
void loaddata()
{
    ifstream fin ;
    fin.open("scene.txt") ;
    //- pushing the floor object beforehand 
    pushFloor() ; 


    fin>>recursion_level ;
    fin>>number_of_pixels ;
    fin>>number_of_objects ;
    for(int i=0 ; i<number_of_objects ; i++)
    {
        string objectType ; 
        fin>>objectType ; 
        if(objectType=="sphere")
        {
            Sphere s ; 
            fin>>s;  
            //s.showSphereInformation() ; 
            Object *temp ; 
            temp = new Sphere(s.center , s.radius , s.color , s.ambient , s.diffuse , s.specular , s.reflection , s.shine) ;
            temp->type = "sphere" ;
            objects.push_back(temp) ;
        }
        else if(objectType=="triangle")
        {
            Triangle t ; 
            fin>>t ;
            //t.showTriangleInformation() ;
            Object *temp ;
            //temp = new Triangle(t.a , t.b , t.c , t.color , t.ambient , t.diffuse , t.specular , t.reflection , t.shine) ;
            temp = new Triangle(t.p1 , t.p2 , t.p3 , t.color , t.ambient , t.diffuse , t.specular , t.reflection , t.shine) ;
            temp->type = "triangle" ;
            objects.push_back(temp) ;


        }
        else if(objectType=="general")
        {
            GeneralQuadSurface gen ; 
            fin>>gen ;
            //gen.showGeneralSurface() ;
            Object *temp ;
            temp = new GeneralQuadSurface(gen.a, gen.b,gen.c,gen.d,gen.e,gen.f,gen.g,gen.h,gen.i,gen.j,gen.cube_reference_point,gen.length,gen.width,gen.height , gen.color , gen.ambient , gen.diffuse , gen.specular , gen.reflection , gen.shine) ;
            temp->type = "general" ;
            objects.push_back(temp) ;
        }
    }
    fin>>number_of_pointLights ;
   // cout<<number_of_pointLights<<endl ;
    for(int i=0 ; i<number_of_pointLights ; i++)
    {
        PointLight pl ; 
        fin>>pl; 
       // pl.showPointLightInformation() ; 
        point_lights.push_back(pl) ; 
    }
    fin>>number_of_spotLights ; 
    for(int i=0 ; i<number_of_spotLights ; i++)
    {
        SpotLight sl ; 
        fin>>sl ; 
      //  sl.showSpotLightInformation() ; 
        spot_lights.push_back(sl) ; 
    }

}  
        

 
int main(int argc , char** argv)
{
    loaddata() ; 
    glutInit(&argc , argv) ; 
    glutInitWindowPosition(1000,200) ; 
    glutInitWindowSize(1000,1000) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; //RGB - Double buffer - depth buffer 
    glutCreateWindow("Ray tracing") ; 
    
    init() ; 
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard); 
    glutDisplayFunc(RayTracing) ;
    glutTimerFunc(50,timer,0);
    glutIdleFunc(idle) ;
    glutMainLoop() ; 
    return 0 ; 
}
