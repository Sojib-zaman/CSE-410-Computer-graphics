#ifndef _1705066_CLASSES_H
#define _1705066_CLASSES_H

#include <bits/stdc++.h>
#include <GL/glut.h>
#include "bitmap_image.hpp"
using namespace std;

#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180
#define toDeg(rad) (double)(rad * 180) / pi

#define COLOR_RED 	0
#define COLOR_GREEN 1
#define COLOR_BLUE 	2

#define N_STACKS 	100
#define N_SLICES 	100

#define COEFF_AMBIENT 		0
#define COEFF_DIFFUSE 		1
#define COEFF_SPECULAR 		2
#define COEFF_REFLECTION 	3

#define EPSILON 		0.0000001
#define DIFF_POSITION 	0.0000000001


class point;
class Ray;
class Color;
class Vector;
class Object;
class Triangle;
class General;
class PointLight;
class SpotLight;
class Sphere;

extern int N_RECURSION;

extern vector<Object *> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;

//  Simple point object for drawing shapes
class point
{
public:
	double x, y, z;
};

class Color
{
public:
	double r, g, b;

	Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}

	Color(double _r, double _g, double _b)
	{
		this->r = _r;
		this->g = _g;
		this->b = _b;
	}

	Color(const Color &color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
	}

	Color operator*(const double value)
	{
		return Color(this->r * value, this->g * value, this->b * value);
	}

	Color operator*(const Color color)
	{
		return Color(this->r * color.r, this->g * color.g, this->b * color.b);
	}

	Color operator+(const Color color)
	{
		return Color(this->r + color.r, this->g + color.g, this->b + color.b);
	}

	//  Wasted 5h just for this fault
	//  Color needs to be between (0-1) for this offline
	void normalize()
	{
		if (r < 0)
			r = 0;
		if (g < 0)
			g = 0;
		if (b < 0)
			b = 0;
		if (r > 1)
			r = 1;
		if (g > 1)
			g = 1;
		if (b > 1)
			b = 1;
	}

	friend ostream &operator<<(ostream &out, Color &color)
	{
		out << fixed << setprecision(7) << color.r << " " << color.g << " " << color.b;
		return out;
	}

	friend ifstream &operator>>(ifstream &in, Color &color)
	{
		in >> color.r >> color.g >> color.b;
		return in;
	}

	static Color set_black()
	{
		Color ret;
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
		return ret;
	}

	static Color set_white()
	{
		Color ret;
		ret.r = 1;
		ret.g = 1;
		ret.b = 1;
		return ret;
	}
};

class Vector
{
public:
	double x, y, z, w;

	Vector() {}

	Vector(double _x, double _y, double _z, double _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	Vector(double _x, double _y, double _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = 1.0;
	}

	Vector(const Vector &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		this->w = p.w;
	}

	void normalize()
	{
		double value = sqrt(x * x + y * y + z * z);
		if (value == 0.0)
		{
			cout << "ERROR Can't divide by zero" << endl;
			return;
		}
		this->x /= value;
		this->y /= value;
		this->z /= value;
	}

	void scale()
	{
		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w /= this->w;
	}

	Vector operator+(const Vector p)
	{
		return Vector(this->x + p.x, this->y + p.y, this->z + p.z);
	}

	Vector operator-(const Vector p)
	{
		return Vector(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	Vector operator*(const double value)
	{
		return Vector(this->x * value, this->y * value, this->z * value);
	}

	double operator*(const Vector p)
	{
		return this->x * p.x + this->y * p.y + this->z * p.z;
	}

	Vector operator^(const Vector p)
	{
		return Vector(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
	}

	friend ostream &operator<<(ostream &out, Vector &p)
	{
		out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
		return out;
	}

	friend ifstream &operator>>(ifstream &in, Vector &p)
	{
		in >> p.x >> p.y >> p.z;
		return in;
	}
};

void drawPointLightSource(Vector pos, Color color)
{
	glColor3d(color.r, color.g, color.b);
	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(1.0, 50, 50);
	}
	glPopMatrix();
}

void drawSpotLightSource(Vector pos, Color color)
{
	drawPointLightSource(pos, color);
	glBegin(GL_LINES);
	{
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				for (int k = -1; k <= 1; k++)
				{
					glVertex3f(pos.x, pos.y, pos.z);
					glVertex3f(pos.x + i * 2.0, pos.y + j * 2.0, pos.z + k * 2.0);
				}
	}
	glEnd();
}

class PointLight
{
public:
	Vector light_pos;
	Color color;

	PointLight() {}
	PointLight(Vector position, Color color)
	{
		this->light_pos = position;
		this->color = color;
	}

	void draw()
	{
		drawPointLightSource(light_pos, color);
	}
};

class SpotLight : public PointLight
{
public:
	Vector light_direction;
	double cutoff_angle;

	SpotLight() {}
	SpotLight(Vector position, Color color, Vector direction, double cutoff_angle)
	{
		this->light_pos = position;
		this->color = color;
		this->light_direction = direction;
		this->cutoff_angle = cutoff_angle;
	}

	void draw()
	{
		drawSpotLightSource(light_pos, color);
	}
};

class Ray
{
public:
	Vector start;
	Vector direction;

	Ray() {}

	Ray(const Ray &ray)
	{
		this->start = ray.start;
		this->direction = ray.direction;
	}

	Ray(Vector _start, Vector _direction)
	{
		this->start = _start;
		this->direction = _direction;
		this->direction.normalize();
	}
};

class Object
{
public:
	Vector ref_point;
	Color color;

	double height, width, length;
	double coEff[4];
	int shine;

	Object() {}
	virtual void draw() {}

	void setColor(Color color)
	{
		this->color = color;
	}

	void setShine(int shine)
	{
		this->shine = shine;
	}

	void setCoEff(double ambient, double diffuse, double specular, double reflection)
	{
		coEff[COEFF_AMBIENT] = ambient;
		coEff[COEFF_DIFFUSE] = diffuse;
		coEff[COEFF_SPECULAR] = specular;
		coEff[COEFF_REFLECTION] = reflection;
	}

	double intersect(Ray &ray, Color &color, int level)
	{
		double t_min = this->getTmin(ray);

		if (level == 0)
			return t_min;

		Vector intPoint;
		intPoint = ray.start + ray.direction * t_min;

		color = this->getColorAt(intPoint) * this->coEff[COEFF_AMBIENT];
		color.normalize();

		Vector normal = this->getNormalAt(intPoint);
		normal.normalize();

		//	----------------------------------
		//	Basic Light Illumination & Casting
		//	----------------------------------
		//	Point Lights

		for (PointLight eachLight : pointLights)
		{
			Vector lightDirection = eachLight.light_pos - intPoint;
			lightDirection.normalize();

			Vector lightPosition;
			lightPosition = intPoint + lightDirection * DIFF_POSITION;

			Ray lightRay(lightPosition, lightDirection);

			Color tempColor;
			bool inShadow = false;
			double t, t_min_original = INFINITY;

			for (Object *eachObject : objects)
			{
				t = eachObject->intersect(lightRay, tempColor, 0);
				if (t > 0 && t < t_min_original)
				{
					t_min_original = t;
				}
			}

			//  If we aren't in the shadow region, we compute
			//  DIFFUSE and SPECULAR componenets ---> R = 2.(L.N)N - L
			//	
			//	lambert val is cos theta. theta is the ang betn L and N		
			//	phong val is cos(ang betn R & V) to the power shine	

			if (t_min < t_min_original)
			{
				Vector R = normal * 2.0 * (normal * lightDirection) - lightDirection;
				R.normalize();

				double lambertValue = max(normal * lightDirection, 0.0);
				double phongValue = max(pow(R * ray.direction, this->shine), 0.0);

				color = color + this->getColorAt(intPoint) * eachLight.color * lambertValue * this->coEff[COEFF_DIFFUSE];
				color.normalize();

				color = color + eachLight.color * phongValue * this->coEff[COEFF_SPECULAR];
				color.normalize();
			}
		}

		//	Spot Lights
		//  https://ogldev.org/www/tutorial21/tutorial21.html

		for (SpotLight eachSpotLight : spotLights)
		{
			Vector lightDirection = eachSpotLight.light_direction;
			lightDirection.normalize();

			Vector lightToIntersect = intPoint - eachSpotLight.light_pos;
			lightToIntersect.normalize();

			double cos_theta = lightDirection * lightToIntersect;
			double theta = toDeg(acos(cos_theta));

			if (theta > eachSpotLight.cutoff_angle) {
				continue;
			}

			//	^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			//	Except the beginning part of the spotLight, 
			//	remaining will be the same as PointLight

			lightDirection = eachSpotLight.light_pos - intPoint;
			lightDirection.normalize();

			Vector lightPosition = intPoint + lightDirection * DIFF_POSITION;
			Ray lightRay(lightPosition, lightDirection);

			Color tempColor;
			double t, t_min_original = INFINITY;

			for (Object *eachObject : objects)
			{
				t = eachObject->intersect(lightRay, tempColor, 0);
				if (t > 0 && t < t_min_original)
				{
					t_min_original = t;
				}
			}

			if (t_min < t_min_original)
			{
				Vector R = normal * 2.0 * (normal * lightDirection) - lightDirection;
				R.normalize();

				double lambertValue = max(normal * lightDirection, 0.0);
				double phongValue = max(pow(R * ray.direction, this->shine), 0.0);

				color = color + this->getColorAt(intPoint) * eachSpotLight.color * lambertValue * this->coEff[COEFF_DIFFUSE];
				color.normalize();

				color = color + eachSpotLight.color * phongValue * this->coEff[COEFF_SPECULAR];
				color.normalize();
			}
		}

		//	----------------------------
		//  Perform recursive reflection
		//	----------------------------
		if (level >= N_RECURSION)
			return t_min;

		Vector refRayDirection;
		refRayDirection = ray.direction - normal * 2.0 * (normal * ray.direction);
		refRayDirection.normalize();

		Vector refRayStart;
		refRayStart = intPoint + refRayDirection * DIFF_POSITION;
		Ray refRay(refRayStart, refRayDirection);

		//  Find out Nearest intersected Object
		//	-----------------------------------
		Color refColor;
		int nearest = -1;
		double t_ref, t_ref_min = INFINITY;

		for (int i = 0; i < objects.size(); i++)
		{
			t_ref = objects[i]->intersect(refRay, refColor, 0);
			if (t_ref > 0 && t_ref < t_ref_min)
			{
				t_ref_min = t_ref;
				nearest = i;
			}
		}

		if (nearest != -1)
		{
			t_ref_min = objects[nearest]->intersect(refRay, refColor, level + 1);
			color = color + refColor * this->coEff[COEFF_REFLECTION];
			color.normalize();
		}

		return t_min;
	}

	virtual Vector getNormalAt(Vector &intPoint)
	{
		return Vector();
	}

	virtual Color getColorAt(Vector &intPoint)
	{
		return this->color;
	}

	virtual double getTmin(Ray &ray)
	{
		return -1.0;
	}
};

class Floor : public Object
{
public:
	double floor_width;
	double tile_width;
	bitmap_image texture;

	Floor() {}

	Floor(double floor_width, double tile_width)
	{
		this->ref_point = Vector(-floor_width / 2, -floor_width / 2, 0);
		this->floor_width = floor_width;
		this->tile_width = tile_width;
		this->length = tile_width;

		texture = bitmap_image("./doge.bmp");
	}

	void draw() override
	{
		int n_grid = (int)floor_width / tile_width;

		for (int row = 0; row < n_grid; row++)
		{
			for (int col = 0; col < n_grid; col++)
			{
				if ((row + col) % 2 == 0)
				{
					glColor3d(0, 0, 0);
				}
				else
				{
					glColor3d(255, 255, 255);
				}

				glBegin(GL_QUADS);
				{
					glVertex3f(ref_point.x + col * tile_width, 			ref_point.y + tile_width * row, 		ref_point.z);
					glVertex3f(ref_point.x + (col + 1) * tile_width, 	ref_point.y + tile_width * row, 		ref_point.z);
					glVertex3f(ref_point.x + (col + 1) * tile_width, 	ref_point.y + tile_width * (row + 1), 	ref_point.z);
					glVertex3f(ref_point.x + col * tile_width, 			ref_point.y + tile_width * (row + 1), 	ref_point.z);
				}
				glEnd();
			}
		}
	}

	double getTmin(Ray &ray)
	{
		Vector normal(0, 0, 1);

		double t = (-1.0) * ((normal * ray.start) / (normal * ray.direction));
		Vector intPoint = ray.start + ray.direction * t;

		if (intPoint.x < ref_point.x || intPoint.x > -ref_point.x)
		{
			return -1;
		}

		if (intPoint.y < ref_point.y || intPoint.y > -ref_point.y)
		{
			return -1;
		}

		return t;
	}

	Vector getNormalAt(Vector &intPoint) override
	{
		return Vector(0, 0, 1);
	}

	Color getColorAt(Vector &intPoint) override
	{
        int row = (int) ( (intPoint.x - ref_point.x) / tile_width );
        int col = (int) ( (intPoint.y - ref_point.y) / tile_width );

		double tile = 0.5;
		double tex  = 1 - tile;
		int c = (row + col) % 2;

		bool TEXTURE_MODE = false;
		if (!TEXTURE_MODE)
		{
			return Color(c, c, c);
		}

		double x = (intPoint.x - ref_point.x ) - tile_width * row;
		double y = (intPoint.y - ref_point.y ) - tile_width * col;
		// cout << __LINE__ << endl;
		// cout << "x, y: " << x << ", " << y << endl;

		int tex_x = (int) (texture.height() * x)/tile_width;
		int tex_y = (int) (texture.width()  * y)/tile_width;
		// cout << __LINE__ << endl;
		// cout << "tex_x, tex_y: " << tex_x << ", " << tex_y << endl;

        unsigned char r, g, b;
        texture.get_pixel(tex_x, tex_y, r, g, b);
		// cout << __LINE__ << endl;

		double color_r = c * tile + (r / 255.0) * tex;
		double color_g = c * tile + (g / 255.0) * tex;
		double color_b = c * tile + (b / 255.0) * tex;

		return Color(color_r, color_g, color_b);
	}
};

class Triangle : public Object
{
public:
	Vector a, b, c;

	Triangle() {}
	Triangle(Vector _a, Vector _b, Vector _c)
	{
		this->a = _a;
		this->b = _b;
		this->c = _c;
	}

	void draw()
	{
		glColor3d(color.r, color.g, color.b);
		glBegin(GL_TRIANGLES);
		{
			glVertex3d(a.x, a.y, a.z);
			glVertex3d(b.x, b.y, b.z);
			glVertex3d(c.x, c.y, c.z);
		}
		glEnd();
	}

	double getTmin(Ray &ray)
	{
		Vector edge_ba = b - a;
		Vector edge_ca = c - a;

		Vector vert = ray.direction ^ edge_ca;
		double const_a = edge_ba * vert;

		//  Ray parallel to the surface of triangle
		if (const_a > -EPSILON && const_a < EPSILON)
		{
			return -1;
		}

		double f = 1.0 / const_a;
		Vector s = ray.start - a;

		double u = f * (s * vert);
		if (u < 0.0 || u > 1.0)
		{
			return -1;
		}

		Vector q = s ^ edge_ba;
		double v = f * (ray.direction * q);
		if (v < 0.0 || (u + v) > 1.0)
		{
			return -1;
		}

		double t = f * (edge_ca * q);
		if (t > EPSILON)
		{
			return t;
		}

		return -1;
	}

	Vector getNormalAt(Vector &intPoint) override
	{
		Vector normal = (b - a) ^ (c - a);
		normal.normalize();
		return normal;
	}
};

class Sphere : public Object
{
public:
	double radius;

	Sphere(Vector center, double radius)
	{
		this->ref_point = center;
		this->radius = radius;
	}

	void draw()
	{
		glColor3d(color.r, color.g, color.b);
		glPushMatrix();
		{
			glTranslated(ref_point.x, ref_point.y, ref_point.z);
			glutSolidSphere(radius, N_SLICES, N_STACKS);
		}
		glPopMatrix();
	}

	double getTmin(Ray &ray)
	{
		Vector sphereOrigin = ray.start - ref_point;

		double a = 1;
		double b = (ray.direction * sphereOrigin) * 2.0;
		double c = (sphereOrigin * sphereOrigin) - radius * radius;

		double det = b * b - 4 * a * c;
		if (det < 0)
		{
			return -1;
		}

		double d = sqrt(det);
		double t1 = (-b - d) / (2 * a);
		double t2 = (-b + d) / (2 * a);

		if (t1 < 0 && t2 < 0)
		{
			return -1;
		}

		if (t1 > 0)
		{
			return t1;
		}

		if (t2 > 0)
		{
			return t2;
		}

		return -1;
	}

	Vector getNormalAt(Vector &intPoint) override
	{
		Vector normal = intPoint - ref_point;
		normal.normalize();
		return normal;
	}
};


//	General Quadric Surface
//	-----------------------
//	More Info: http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html


class General : public Object
{
public:
	double A, B, C, D, E, F, G, H, I, J;

	General() {}
	General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, double length, double width, double height, Vector point)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		this->D = D;
		this->E = E;
		this->F = F;
		this->G = G;
		this->H = H;
		this->I = I;
		this->J = J;

		this->length = length;
		this->width = width;
		this->height = height;
		this->ref_point = point;
	}

	void draw() {}

	bool isWithinRange(Vector point)
	{
		if (length != 0)
		{
			if (point.x < ref_point.x || point.x > ref_point.x + length)
				return false;
		}
		if (width != 0)
		{
			if (point.y < ref_point.y || point.y > ref_point.y + width)
				return false;
		}
		if (height != 0)
		{
			if (point.z < ref_point.z || point.z > ref_point.z + height)
				return false;
		}
		return true;
	}

	/**
	 *
		F(x, y, z) = Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0
		Then substitute in ray equation R(t) = Ro + Rd and we get quadratic equation:

		Aq.t2 + Bq.t + Cq = 0 with

		Aq = A.xd2    + B.yd2   + C.zd2 +
			+ D.xd.yd + E.xd.zd + F.yd.zd

		Bq = 2*A.xo.xd          + 2*B.yo.yd         + 2*C.zo.zd         +
			+ D(xo.yd + yo.xd)  + E(xo.zd + zo.xd)  + F(yo.zd + yd.zo)  +
			+ G.xd              + H.yd              + I.zd

		Cq = A.xo2 + B.yo2 + C.zo2 +
			+ D.xo.yo + E.xo.zo + F.yo.zo +
			+ G.xo + H.yo + I.zo + J
	*/

	double getTmin(Ray &ray) override
	{
		double xo = ray.start.x, xd = ray.direction.x;
		double yo = ray.start.y, yd = ray.direction.y;
		double zo = ray.start.z, zd = ray.direction.z;

		//  **********************************************************
		//  ********************  CAREFUL SPACE  *********************
		//  **********************************************************

		double aq = 0;
		aq += A * pow(xd, 2) + B * pow(yd, 2) + C * pow(zd, 2);
		aq += D * xd * yd + E * xd * zd + F * yd * zd;

		double bq = 0;
		bq += 2 * A * xo * xd + 2 * B * yo * yd + 2 * C * zo * zd;
		bq += D * (xo * yd + yo * xd) + E * (xo * zd + zo * xd) + F * (yo * zd + yd * zo);
		bq += G * xd + H * yd + I * zd;

		double cq = 0;
		cq += A * pow(xo, 2) + B * pow(yo, 2) + C * pow(zo, 2);
		cq += D * (xo * yo) + E * (xo * zo) + F * (yo * zo);
		cq += G * xo + H * yo + I * zo + J;

		//  **********************************************************
		//  ********************  CAREFUL SPACE  *********************
		//  **********************************************************
		//  wasted around 2hr behind this thing

		double det = bq * bq - 4 * aq * cq;
		if (det < 0)
		{
			return -1;
		}

		double d = sqrt(det);
		double t1 = (-bq - d) / (2 * aq);
		double t2 = (-bq + d) / (2 * aq);

		Vector point_1 = ray.start + ray.direction * t1;
		Vector point_2 = ray.start + ray.direction * t2;

		if (t1 < 0 && t2 < 0)
		{
			return -1;
		}
		if (t1 > 0 && isWithinRange(point_1))
		{
			return t1;
		}
		if (t2 > 0 && isWithinRange(point_2))
		{
			return t2;
		}

		return -1;
	}

	/*
	 *   Equation : F(x,y,z) = Ax2 + By2 + Cz2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
	 *   Normal (Partial derivative):
	 *   -----------------------------
	 *       dF/dx = 2Ax + Dy + Ez + G
	 *       dF/dy = 2By + Dx + Fz + H
	 *       dF/dz = 2Cz + Ex + Fy + I
	 */

	Vector getNormalAt(Vector &intPoint) override
	{
		double df_dx = 2 * A * intPoint.x + D * intPoint.y + E * intPoint.z + G;
		double df_dy = 2 * B * intPoint.y + D * intPoint.x + F * intPoint.z + H;
		double df_dz = 2 * C * intPoint.z + E * intPoint.x + F * intPoint.y + I;

		Vector normal(df_dx, df_dy, df_dz);
		normal.normalize();
		return normal;
	}
};

#endif