#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#include "1705066_classes.hpp"
#include "bitmap_image.hpp"

#define GL_WINDOW_HEIGHT 	500
#define GL_WINDOW_WIDTH 	500
#define GL_VIEW_ANGLE 		80

#define RGB_LIMIT 			255

#define CAMERA_ROTATE_STEP 	5
#define CAMERA_MOVE_STEP 	10

#define INPUT_FILE "./scene.txt"


Vector pos;
Vector l, r, u;

int drawaxes;
int imageCount = 0;
int IMAGE_WIDTH = 0;
int N_RECURSION = 0;

vector<Object *> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 0, 0);
		glBegin(GL_LINES);
		{
			glVertex3f(500, 0, 0);
			glVertex3f(-500, 0, 0);
		}
		glEnd();

		glColor3f(0, 1.0, 0);
		glBegin(GL_LINES);
		{
			glVertex3f(0, 500, 0);
			glVertex3f(0, -500, 0);
		}
		glEnd();

		glColor3f(0, 0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(0, 0, 500);
			glVertex3f(0, 0, -500);
		}
		glEnd();
	}
}

void capture()
{
	cout << "Capturing Image" << endl;

	//	In Bitmap, column comes first
	bitmap_image image(IMAGE_WIDTH, IMAGE_WIDTH);
	for (int col = 0; col < IMAGE_WIDTH; col++)
	{
		for (int row = 0; row < IMAGE_WIDTH; row++)
		{
			image.set_pixel(col, row, 0, 0, 0);
		}
	}

	double planeDistance;
	planeDistance = (GL_WINDOW_HEIGHT / 2.0) / tan(toRad(GL_VIEW_ANGLE / 2.0));

	Vector topLeft;
	topLeft = pos + (l * planeDistance) - (r * (GL_WINDOW_WIDTH / 2.0)) + (u * (GL_WINDOW_HEIGHT / 2.0));

	double du = (double)GL_WINDOW_WIDTH / IMAGE_WIDTH;
	double dv = (double)GL_WINDOW_HEIGHT / IMAGE_WIDTH;

	//	Choosing middle of the grid
	topLeft = topLeft + (r * (0.5 * du)) - (u * (0.5 * dv));

	int nearest;
	double t, t_min;
	Vector currentPixel{};

	for (int col = 0; col < IMAGE_WIDTH; col++)
	{
		for (int row = 0; row < IMAGE_WIDTH; row++)
		{
			nearest = -1;
			t_min = INFINITY;
			currentPixel = topLeft + (r * (col * du)) - (u * (row * dv));

			Ray ray(pos, (currentPixel - pos));
			Color color;

			for (int i = 0; i < objects.size(); i++)
			{
				t = objects[i]->intersect(ray, color, 0);
				if (t > 0 && t < t_min)
				{
					t_min = t;
					nearest = i;
				}
			}

			if (nearest != -1)
			{
				t_min = objects[nearest]->intersect(ray, color, 1);
			}
			color.normalize();
			image.set_pixel(col, row, color.r * RGB_LIMIT, color.g * RGB_LIMIT, color.b * RGB_LIMIT);
		}
	}
	string imageName = "Output_1" + to_string(imageCount) + ".bmp";
	imageCount++;
	image.save_image(imageName);
	image.clear();
	cout << "Image '" + imageName + "' saved!" << endl;
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		//	Rotate Left
		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) - r.x * sin(toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) - r.y * sin(toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) - r.z * sin(toRad(CAMERA_ROTATE_STEP));

		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) + l.x * sin(toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) + l.y * sin(toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) + l.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '2':
		//	Rorate right
		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) - r.x * sin(-toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) - r.y * sin(-toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) - r.z * sin(-toRad(CAMERA_ROTATE_STEP));

		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) + l.x * sin(-toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) + l.y * sin(-toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) + l.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	case '3':
		//	Rotate Up
		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) - l.x * sin(toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) - l.y * sin(toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) - l.z * sin(toRad(CAMERA_ROTATE_STEP));

		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) + u.x * sin(toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) + u.y * sin(toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) + u.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '4':
		//	Rotate Down
		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) - l.x * sin(-toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) - l.y * sin(-toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) - l.z * sin(-toRad(CAMERA_ROTATE_STEP));

		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) + u.x * sin(-toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) + u.y * sin(-toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) + u.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	case '5':
		//	Rotate Clockwise
		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) - u.x * sin(toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) - u.y * sin(toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) - u.z * sin(toRad(CAMERA_ROTATE_STEP));

		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) + r.x * sin(toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) + r.y * sin(toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) + r.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '6':
		//	Rotate CounterClockwise
		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) - u.x * sin(-toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) - u.y * sin(-toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) - u.z * sin(-toRad(CAMERA_ROTATE_STEP));

		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) + r.x * sin(-toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) + r.y * sin(-toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) + r.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	case '0':
		capture();
		break;

	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		//	Move Back
		pos.x -= l.x * CAMERA_MOVE_STEP;
		pos.y -= l.y * CAMERA_MOVE_STEP;
		pos.z -= l.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_UP:
		//	Move Front
		pos.x += l.x * CAMERA_MOVE_STEP;
		pos.y += l.y * CAMERA_MOVE_STEP;
		pos.z += l.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_RIGHT:
		//	Move Right
		pos.x += r.x * CAMERA_MOVE_STEP;
		pos.y += r.y * CAMERA_MOVE_STEP;
		pos.z += r.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_LEFT:
		//	Move Left
		pos.x -= r.x * CAMERA_MOVE_STEP;
		pos.y -= r.y * CAMERA_MOVE_STEP;
		pos.z -= r.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_PAGE_UP:
		//	Move Up
		pos.x += u.x * CAMERA_MOVE_STEP;
		pos.y += u.y * CAMERA_MOVE_STEP;
		pos.z += u.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_PAGE_DOWN:
		//	Move Down
		pos.x -= u.x * CAMERA_MOVE_STEP;
		pos.y -= u.y * CAMERA_MOVE_STEP;
		pos.z -= u.z * CAMERA_MOVE_STEP;
		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		break;

	default:
		break;
	}
}

void display()
{
	// clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); // color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	// load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	// initialize the matrix
	glLoadIdentity();

	// now give three info
	// 1. where is the camera (viewer)?
	// 2. where is the camera looking?
	// 3. Which direction is the camera's UP direction?

	gluLookAt(pos.x, pos.y, pos.z,
			  pos.x + l.x, pos.y + l.y, pos.z + l.z,
			  u.x, u.y, u.z);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	drawAxes();

	for (Object *each : objects)
	{
		each->draw();
	}

	for (PointLight light : pointLights)
	{
		light.draw();
	}

	for (SpotLight light : spotLights)
	{
		light.draw();
	}

	glutSwapBuffers();
}

void animate()
{
	// codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	// codes for initialization
	drawaxes = 1;

	// clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	pos.x = 200;
	pos.y = 200;
	pos.z = 0;

	l.x = -1 / sqrt(2.00);
	l.y = -1 / sqrt(2.00);
	l.z = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1 / sqrt(2.00);
	r.y = 1 / sqrt(2.00);
	r.z = 0;
	//	Initialization Done

	//	Crearing Screen
	glClearColor(0, 0, 0, 0);

	// load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	// initialize the matrix
	glLoadIdentity();

	// give PERSPECTIVE parameters
	gluPerspective(GL_VIEW_ANGLE, 1, 1, 1000.0);
	// field of view in the Y (vertically)
	// aspect ratio that determines the field of view in the X direction (horizontally)
	// near distance
	// far distance
}

void loadData()
{
	ifstream input;
	input.open(INPUT_FILE);

	if (!input.is_open())
	{
		cout << "The file '" << INPUT_FILE << "' doesn't exist!" << endl;
		exit(0);
	}

	//	Arbitrarily Constants
	input >> N_RECURSION >> IMAGE_WIDTH;

	Object *object;
	string objectType;

	int nObjects;
	input >> nObjects;

	for (int i = 0; i < nObjects; i++)
	{
		input >> objectType;

		if (objectType == "sphere")
		{
			Vector center;
			input >> center;

			double radius;
			input >> radius;

			Color color;
			input >> color;

			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new Sphere(center, radius);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else if (objectType == "triangle")
		{
			Vector a, b, c;
			input >> a >> b >> c;

			Color color;
			input >> color;

			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new Triangle(a, b, c);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else if (objectType == "general")
		{
			double A, B, C, D, E, F, G, H, I, J;
			input >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

			Vector point;
			input >> point;

			double length, width, height;
			input >> length >> width >> height;

			Color color;
			input >> color;

			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new General(A, B, C, D, E, F, G, H, I, J, length, width, height, point);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else
		{
			cout << "Object Type '" << objectType << "' is invalid!" << endl;
			break;
		}
	}

	object = new Floor(800, 20);
	object->setCoEff(0.5, 0.3, 0.4, 0.2);
	object->setShine(10);
	objects.push_back(object);

	int nPointLights;
	input >> nPointLights;

	for (int i = 0; i < nPointLights; i++)
	{
		Vector position;
		input >> position;

		Color color;
		input >> color;

		PointLight light(position, color);
		pointLights.push_back(light);
	}

	int nSpotLights;
	input >> nSpotLights;

	for (int i = 0; i < nSpotLights; i++)
	{
		Vector position;
		input >> position;

		Color color;
		input >> color;

		Vector direction;
		input >> direction;

		double cutoff_angle;
		input >> cutoff_angle;

		SpotLight light(position, color, direction, cutoff_angle);
		spotLights.push_back(light);
	}

	cout << endl << ">>> LOAD DATA COMPLETED! <<<" 			<< endl;
	cout << "--------------------------------------------" 	<< endl;
	cout << "Total Objects: " << objects.size() 			<< endl;
	cout << "Total Point Lights: " << pointLights.size() 	<< endl;
	cout << "Total Spot Lights: " << spotLights.size() 		<< endl;
	cout << "--------------------------------------------" 	<< endl;
}

int main(int argc, char **argv)
{
	loadData();

	glutInit(&argc, argv);
	glutInitWindowSize(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("1705066 - Task 1");
	init();

	glEnable(GL_DEPTH_TEST); // enable Depth Testing

	glutDisplayFunc(display); // display callback function
	glutIdleFunc(animate);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop(); // The main loop of OpenGL

	cout << endl << "Memory Cleanup Started" << endl;

	for (int i=0; i < objects.size(); i++) {
		delete objects[i];
	}
	vector<Object*>().swap(objects);
	vector<PointLight>().swap(pointLights);
	vector<SpotLight>().swap(spotLights);

	cout << "Memory Clearnup Completed!" << endl;

	return 0;
}
