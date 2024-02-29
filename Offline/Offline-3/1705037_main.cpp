#include "1705037_classes.h"


extern vector <Object*> objects;
extern vector <PointLight> pointLights;
extern vector <SpotLight> spotLights;

extern int recursionLevel;
int pixels=0;
int num_of_objects=0;
int num_point_lights=0;
int num_spotlights=0;
int img_cap=1;

double cameraHeight;
double cameraAngle;
double theta;
int drawgrid;
int drawaxes;
double angle;



//Vector3D eye = {0.707*150, 0, -0.707*150};
//Vector3D u = {0, 1, 0};
//Vector3D r = {0.707,0, 0.707};
//Vector3D l = {-0.707, 0, 0.707};

Vector3D eye(150, 150, 0);
Vector3D u(0, 0, 1);
Vector3D r(-0.707,0.707, 0);
Vector3D l(-0.707, -0.707, 0);


Vector3D cubePoints[8];
double total_length=50;
double radius=20;
double height;

double windowHeight = 500;
double windowWidth = 500;

void clear_memory()
{
    cout << "clearing memory" << endl;
    for (auto p : objects)
    {
        delete p;
    }
    objects.clear();
    pointLights.clear();
    spotLights.clear();
}


void capture()
{
   cout << "capturing image " << img_cap << endl;
   bitmap_image image(pixels,pixels);


   double planeDistance = (windowHeight/2.0)/tan((cameraAngle/2.0)*(pi/180));
   Vector3D topleft;
   topleft.x = eye.x + (l.x)*planeDistance - (r.x)*windowWidth/2 + (u.x)*windowHeight/2;
   topleft.y = eye.y  + (l.y )*planeDistance - (r.y )*windowWidth/2 + (u.y )*windowHeight/2;
   topleft.z = eye.z + (l.z)*planeDistance - (r.z)*windowWidth/2 + (u.z)*windowHeight/2;
   double imageWidth,imageHeight;
   imageHeight = imageWidth = pixels;

   double du = (double) ((windowWidth*1.0)/(imageWidth*1.0));
   double dv = (double) ((windowHeight*1.0)/(imageHeight*1.0));
   topleft.x = topleft.x + r.x*(0.5*du) - u.x*(0.5*dv);
   topleft.y = topleft.y + r.y*(0.5*du) - u.y*(0.5*dv);
   topleft.z = topleft.z + r.z*(0.5*du) - u.z*(0.5*dv);
   int nearest;
   double t,tMin;
   for(int i=0;i<pixels;i++)
   {
       for(int j=0;j<pixels;j++)
       {
           //calculate curPixel using topleft,r,u,i,j,du,dv
           //topLeft+r*(column*du)-u*(row*dv)
           image.set_pixel(i,j,0,0,0);
           Vector3D curPixel;
           curPixel.x = topleft.x + r.x*i*du - u.x*j*dv;
           curPixel.y = topleft.y + r.y*i*du - u.y*j*dv;
           curPixel.z = topleft.z + r.z*i*du - u.z*j*dv;
//            cout << curPixel.x << " " << curPixel.y << " " << curPixel.z << endl;
           //cast ray from eye to (curPixel-eye) direction

           Vector3D rd;
           rd.x = curPixel.x-eye.x;
           rd.y = curPixel.y-eye.y;
           rd.z = curPixel.z-eye.z;

//            cout << rd.x << " " << rd.y << " " << rd.z << endl;

           Ray *ray = new Ray(eye,rd);

           double *color;
//           double *color = new double[3];
//           for(int k=0;k<3;k++)
//           {
//               color[k]=0;
//           }

//            cout << "reaches 0" << endl;
           tMin = INT_MAX;
           nearest = 0;
           int idx=0;
           for (auto & o : objects)
           {
               //for each object, o in objects

               t = o->intersect(ray, color, 0);
//                if(t>=0)
//                {
//                    if(o->getType()=="triangle")
//                    {
//                        cout << i << j << ":" << endl;
//                        cout << o->getType() << endl;
//                        double *dummycolor = o->getColor();
//                        cout << dummycolor[0] << dummycolor[1] << dummycolor[2] << endl;
//                        cout << "t " << t << endl;
//                        cout << endl;
//                        cout << endl;
//                        cout << endl;
//                    }
//
//                }
//                update t so that it stores min +ve value
               if(t<tMin && t>0)
               {
                   tMin=t;
                   nearest = idx;
               }

//                save the nearest object, on
//                t
//                min = on->intersect(ray, color, 1)
//                update image pixel (i,j)
               idx++;
           }
           if(tMin!=INT_MAX)
           {
                Object* n = objects[nearest];
                tMin = n->intersect(ray,color,1);
                color = n->getColorBmp();
                image.set_pixel(i,j,color[0]*255.0,color[1]*255.0,color[2]*255.0);
           }

        delete ray;

       }
   }

   image.save_image("Output_1"+std::to_string(img_cap)+".bmp");
   cout << "captured image " << img_cap << endl;
   img_cap++;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f(100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    Vector3D points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    Vector3D points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}




void drawSphere(double radius,int slices,int stacks)
{
    glColor3f(1,0,0);
	Vector3D points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*1/2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*1/2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double cylh,int slices,int stacks)
{
    glColor3f(0,1,0);
	Vector3D points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=cylh/2*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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



void drawSS()
{
    Object* floor = new Floor(1000,20);
    objects.push_back(floor);
    for (auto & element : objects)
    {
       glPushMatrix();
       glTranslatef(element->reference_point.x,element->reference_point.y,element->reference_point.z);
       element->draw();
       glPopMatrix();
    }
    for (auto & element : pointLights)
    {
       element.draw();
    }
    for (auto & element : spotLights)
    {
       element.draw();
    }

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
    //look left->clockwise rotation about u
        case '0':
            capture();
            break;
        case '1':
            theta = 0.03;
			l.x = l.x*cos(theta)-r.x*sin(theta);
			l.y = l.y*cos(theta)-r.y*sin(theta);
			l.z = l.z*cos(theta)-r.z*sin(theta);

			r.x = l.x*sin(theta)+r.x*cos(theta);
			r.y = l.y*sin(theta)+r.y*cos(theta);
			r.z = l.z*sin(theta)+r.z*cos(theta);
			break;
    //look right->anticlockwise rotation about u
        case '2':
            theta = -0.03;
			l.x = l.x*cos(theta)-r.x*sin(theta);
			l.y = l.y*cos(theta)-r.y*sin(theta);
			l.z = l.z*cos(theta)-r.z*sin(theta);

			r.x = l.x*sin(theta)+r.x*cos(theta);
			r.y = l.y*sin(theta)+r.y*cos(theta);
			r.z = l.z*sin(theta)+r.z*cos(theta);
			break;

    //look up->clockwise rotation about r
       case '3':
            theta = 0.03;
			l.x = l.x*cos(theta)-u.x*sin(theta);
			l.y = l.y*cos(theta)-u.y*sin(theta);
			l.z = l.z*cos(theta)-u.z*sin(theta);

			u.x = l.x*sin(theta)+u.x*cos(theta);
			u.y = l.y*sin(theta)+u.y*cos(theta);
			u.z = l.z*sin(theta)+u.z*cos(theta);
			break;
    //look down->anticlockwise rotation about r
        case '4':
            theta = -0.03;
			l.x = l.x*cos(theta)-u.x*sin(theta);
			l.y = l.y*cos(theta)-u.y*sin(theta);
			l.z = l.z*cos(theta)-u.z*sin(theta);

			u.x = l.x*sin(theta)+u.x*cos(theta);
			u.y = l.y*sin(theta)+u.y*cos(theta);
			u.z = l.z*sin(theta)+u.z*cos(theta);
			break;
    //tilt clockwise->anticlockwise rotation about l
        case '5':
            theta = -0.03;
			u.x = u.x*cos(theta)-r.x*sin(theta);
			u.y = u.y*cos(theta)-r.y*sin(theta);
			u.z = u.z*cos(theta)-r.z*sin(theta);

			r.x = u.x*sin(theta)+r.x*cos(theta);
			r.y = u.y*sin(theta)+r.y*cos(theta);
			r.z = u.z*sin(theta)+r.z*cos(theta);
			break;
    //tilt anticlockwise->clockwise rotation about l
        case '6':
            theta = 0.03;
			u.x = u.x*cos(theta)-r.x*sin(theta);
			u.y = u.y*cos(theta)-r.y*sin(theta);
			u.z = u.z*cos(theta)-r.z*sin(theta);

			r.x = u.x*sin(theta)+r.x*cos(theta);
			r.y = u.y*sin(theta)+r.y*cos(theta);
			r.z = u.z*sin(theta)+r.z*cos(theta);
			break;



		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			move along negative l
			eye.x = eye.x - 3*l.x;
			eye.y = eye.y - 3*l.y;
			eye.z = eye.z - 3*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
//			move along positive l
			eye.x = eye.x + 3*l.x;
			eye.y = eye.y + 3*l.y;
			eye.z = eye.z + 3*l.z;
			break;

		case GLUT_KEY_RIGHT:
//			move along negative r
            eye.x = eye.x + 3*r.x;
			eye.y = eye.y + 3*r.y;
			eye.z = eye.z + 3*r.z;
			break;
		case GLUT_KEY_LEFT:
//			move along positive r
            eye.x = eye.x - 3*r.x;
			eye.y = eye.y - 3*r.y;
			eye.z = eye.z - 3*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
//			move along positive u
            eye.x = eye.x + 3*u.x;
			eye.y = eye.y + 3*u.y;
			eye.z = eye.z + 3*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
//			move along negative u
            eye.x = eye.x - 3*u.x;
			eye.y = eye.y - 3*u.y;
			eye.z = eye.z - 3*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(radius<total_length)
            {
                radius+=1.0;
                height=total_length-radius;
            }

			break;
		case GLUT_KEY_END:
		    if(radius>0)
            {
                radius-=1.0;
                height=total_length-radius;
            }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(pos.x,pos.y,pos.z,	0,0,0,	0,1,0);
    gluLookAt(eye.x,eye.y,eye.z,	eye.x+l.x,eye.y+l.y,eye.z+
              l.z,	u.x,u.y,u.z);
//

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
//    glRotatef(-90,1,0,0);
	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=80.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(cameraAngle,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void loadData()
{
    std::ifstream in("scene.txt");
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to scene.txt

    cin >> recursionLevel;
    cin >> pixels;
    cin >> num_of_objects;

//    cout << recursionLevel << " " << pixels << " " << num_of_objects << endl;

    string object_type;
    double color[3];
    double ambient, diffuse, specular, recursive_reflection_coefficient;
    int shine;

    for(int i=0;i<num_of_objects;i++)
    {
        Object *obj;
        cin >> object_type;
        if(object_type=="sphere")
        {
            Vector3D *center = new Vector3D();
            cin >> center->x >> center->y >> center->z;

            double radius;

            cin >> radius;
            cin >> color[0] >> color[1] >> color[2];
            cin >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;
            cin >> shine;

            obj = new Sphere(*center,radius);
            obj->setColor(color[0],color[1],color[2]);
            obj->setCoEfficients(ambient, diffuse, specular, recursive_reflection_coefficient);
            obj->setShine(shine);

        }
        else if(object_type=="triangle")
        {
            Vector3D* points[3];
            for(int j=0;j<3;j++)
            {
                points[j] = new Vector3D();
                cin >> points[j]->x >> points[j]->y >> points[j]->z;
            }
            cin >> color[0] >> color[1] >> color[2];
            cin >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;
            cin >> shine;

            obj = new Triangle(*points[0],*points[1],*points[2]);
            obj->setColor(color[0],color[1],color[2]);
            obj->setCoEfficients(ambient, diffuse, specular, recursive_reflection_coefficient);
            obj->setShine(shine);
        }
        else
        {
            double gen_coeffs[10],length,width,height;
            Vector3D *ref_point = new Vector3D();
            for(int j=0;j<10;j++)
            {
                cin >> gen_coeffs[j];
            }
            cin >> ref_point->x >> ref_point->y >> ref_point->z >> length >> width >> height;
            cin >> color[0] >> color[1] >> color[2];
            cin >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;
            cin >> shine;

            obj = new Object(gen_coeffs,*ref_point,length,width,height);
            obj->setColor(color[0],color[1],color[2]);
            obj->setCoEfficients(ambient, diffuse, specular, recursive_reflection_coefficient);
            obj->setShine(shine);
        }
        obj->setType(object_type);
        objects.push_back(obj);
    }

    cin >> num_point_lights;
    for(int i=0;i<num_point_lights;i++)
    {
        PointLight pointlight;
        Vector3D pos;
        double color[3];
        cin >> pos.x >> pos.y >> pos.z;
        cin >> color[0] >> color[1] >> color[2];
        for(int j=0;j<3;j++)
        {
            pointlight.color[j] = color[j];
        }
        pointlight.light_pos = pos;
        pointLights.push_back(pointlight);
    }

    cin >> num_spotlights;
    for(int i=0;i<num_spotlights;i++)
    {
        SpotLight spotlight;
        Vector3D pos,dir;
        double color[3],cutoff_angle;
        cin >> pos.x >> pos.y >> pos.z;
        cin >> color[0] >> color[1] >> color[2];
        cin >> dir.x >> dir.y >> dir.z;
        cin >> cutoff_angle;

        for(int j=0;j<3;j++)
        {
            spotlight.point_light.color[j] = color[j];
        }
        spotlight.point_light.light_pos = pos;
        spotlight.light_direction = dir;
        spotlight.cutoff_angle = cutoff_angle;

        spotLights.push_back(spotlight);
    }


}


int main(int argc, char **argv){

    loadData();

	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

	init();
    glEnable(GL_CLIP_PLANE0);
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);
    atexit(clear_memory);
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}


