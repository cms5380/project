#include "move.h"

Drone::Drone(void) {
	setDefaultCamera();
}
void Drone::setDefaultCamera(void) {
	Point dronePoint = Point(0.0, 0.0, 0.0);
	set(dronePoint);
}
void Drone::set(float pX, float pY, float pZ) {

	Point Drone = Point(pX, pY, pZ);
	drone.set(Drone);
	setMatrix();
}
void Drone::set(Point Drone) {
	drone.set(Drone);
	setMatrix();
}
void Drone::setMatrix(void) {
	m[0] = y.x;	m[4] = y.y;	m[8] = y.z;	m[12] = -drone.x;
	m[1] = z.x;	m[5] = z.y;	m[9] = z.z;	m[13] = -drone.y;
	m[2] = x.x;	m[6] = x.y;	m[10] = x.z;	m[14] = -drone.z;
	m[3] = 0;		m[7] = 0;		m[11] = 0;	m[15] = 1.0;
	glMultMatrixf(m);
}
void Drone::trans(double dx, double dy, double dz){ 
	drone.x += dy*z.x + dx*y.x + dz*x.x;
	drone.y += dy*z.y + dx*y.y + dz*x.y;
	drone.z += dy*z.z + dx*y.z + dz*x.z;
	setMatrix();
}
void Drone::rota(float angle) {
	ro_cul(y, z, angle);
	setMatrix();
}
void Drone::ro_cul(Vector& a, Vector& b, float spin) {
	float PI = atan(1) * 4;
	float angle = PI / 180 * spin;
	Vector t(cos(angle)*a.x + sin(angle)*b.x, cos(angle)*a.y + sin(angle)*b.y, cos(angle)*a.z + sin(angle)*b.z);
	b.set(-sin(angle)*a.x + cos(angle)*b.x, -sin(angle)*a.y + cos(angle)*b.y, -sin(angle)*a.z + cos(angle)*b.z);
	a.set(t.x, t.y, t.z);
}