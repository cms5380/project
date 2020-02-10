#pragma once
#include <windows.h>
#include <math.h>
#include <gl/glut.h>

class Point {
public:
	float x, y, z;
	Point(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	Point() { x = y = z = 0; }
	void set(Point& p) { x = p.x; y = p.y; z = p.z; }
};
class Vector {
public:
	float x, y, z;
	Vector(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	Vector() { x = 0; y = 0; z = 0;}
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
};
class Drone {
public:
	Point drone;
	Vector x, y, z;
	GLfloat m[16];

	Drone(void);

	void setMatrix();
	void rota(float angle);
	void trans(double du, double dv, double dn);
	void ro_cul(Vector& a, Vector& b, float angle);
	void setDefaultCamera();
	void set(Point Drone);
	void set(float pX, float pY, float pZ);
};