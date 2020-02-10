#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <GL/glut.h>
#include <iostream>
#include<windows.h>
#include<math.h>
#include<GL/GLAUX.H>
#include<GL/glext.h>
#include"move.h"

Drone dro;
GLUquadric *m_pQuadric;
GLuint g_nCubeTex;


void init(void);
void draw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void myKeyboardUp(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);
void sub_menu_function(int option);
//void sub_menu_draw(int option);
void main_menu_function(int option);
void draw_axis(void);
void resize(int width, int height);
void idle(void);
void file_load(char *fname);
//void get_normalvector();
void draw_d(void);
void drow_tri();
void draw_text();
void draw_string(void *font, const char* str, float x_position, float y_position, float red, float green, float blue);
void draw_disp(void);

char ch[1000];
float x, y, z, u, v;
int i, j, k, l;
float v_x[11][10000], v_y[11][10000], v_z[11][10000];
float vtu[11][10000], vtv[11][10000];
float nvx[11][10000], nvy[11][10000], nvz[11][10000];
int f_v1[11][10000], f_v2[11][10000], f_v3[11][10000];
int f_vt1[11][10000], f_vt2[11][10000], f_vt3[11][10000];
int f_vn1[11][10000], f_vn2[11][10000], f_vn3[11][10000];
int f_num;
int v1, v2, v3, vn1, vn2, vn3, vt1, vt2, vt3;
int d_num = 0;

float angle1 = 0;

float PI = atan(1) * 4;
int eye = 0;
int px = 130, py = 130, pz = 130;
double distance = sqrt(px*px + py*py + pz*pz);
double eyex, eyey, eyez;
double elevation;
double azumuth;
double El, Az;
double upy, upx, upz;
float spinf = 0, spinl = 0, legspin = 0, wingspin = 0;
float fly_dron = 0;
int moux = 0, mouy = 0;

double camerapx = 0, camerapy = 50, camerapz = 2.199;
double cameradistance = sqrt(camerapx*camerapx + camerapy*camerapy + camerapz*camerapz);
double cameraeyex, cameraeyey, cameraeyez;
double cameraelevation;
double cameraazumuth;
double cameraEl, cameraAz;
double cameraupy;

int sight = 0;
float wing_speed = 0;
int fly_speed = 0;
int move;
float move_f;
float move_l;
float move_up;
float move_spin = 0;
float move__;
float rotate_cam = 0;
float rotate_cam1 = 0;
bool spin_w = false;
bool spin_l = false;
bool moving_f = false, moving_b = false, moving_l = false, moving_r = false;
bool keyPressed[256];
bool mouseScale = false, mouseEyetrans = false;
float tmp, tmp1, tmp2, tmp3;
float wingZ = 6.727;
float legZ = 1.119;
float l_legX = -10.138; //11.331
float r_legX = 10.138;
float r_wingX = 10.149;
float l_wingX = -10.149;
GLuint texName[6];
GLuint base;
int mouse_x, mouse_y;
int scale;
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //그려지고 회전
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //회전하고 그림
	glutInitWindowSize(900, 720);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("12121710 Choi min seok");
	glutReshapeFunc(resize);
	init();

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	glutSpecialFunc(specialkeyboard);

	glutDisplayFunc(draw);

	/*submenul = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("sub1", 1);
	glutAddMenuEntry("sub2", 2);
	glutAddMenuEntry("sub3", 3);*/

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);
	//glutAddSubMenu("Sub Menu", submenul);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutIdleFunc(idle);

	glutMainLoop();


}
void draw_axis(void) {
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
}

void init(void) {
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//-----------------------------------------------------------
	//물체의 색깔을 glColor3f();로 설정 할 수 있습니다. 그래서
	//행성의 색깔이 흰색이 아니라 각각의 색깔로 표현 가능합니다.
	//-----------------------------------------------------------
	GLfloat ambientLight[] = { 0.01f,0.01f,0.01f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat specularLight[] = { 0.9f,0.9f,0.9f,1.0f };
	GLfloat specularMaterial[] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);

	GLfloat ambientLight1[] = { 0.6f,0.6f,0.6f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);

	GLfloat diffuseLight2[] = { 0.7f,0.7f,0.7f,1.0f };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);


	//-----------------------------------------------------------

	d_num = 1;
	file_load("wing.obj");
	d_num = 2;
	file_load("l_leg0.obj");
	d_num = 3;
	file_load("r_leg0.obj");
	d_num = 4;
	file_load("l_leg1.obj");
	d_num = 5;
	file_load("r_leg1.obj");
	d_num = 6;
	file_load("camera.obj");
	d_num = 7;
	file_load("Cylin.obj");
	d_num = 8;
	file_load("cam_l.obj");
	d_num = 9;
	file_load("bat.obj");
	d_num = 10;
	file_load("bodyc.obj");
	d_num = 0;
	file_load("bodynew.obj");
	m_pQuadric = gluNewQuadric();
	AUX_RGBImageRec * Img[6];

	glGenTextures(1, &g_nCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	Img[0] = auxDIBImageLoadA("512px.bmp");
	Img[1] = auxDIBImageLoadA("512nx.bmp");
	Img[2] = auxDIBImageLoadA("512py.bmp");
	Img[3] = auxDIBImageLoadA("512ny.bmp");
	Img[4] = auxDIBImageLoadA("512pz.bmp");
	Img[5] = auxDIBImageLoadA("512nz.bmp");

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, Img[0]->sizeX, Img[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[0]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, Img[1]->sizeX, Img[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[1]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, Img[2]->sizeX, Img[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[2]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, Img[3]->sizeX, Img[3]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[3]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, Img[4]->sizeX, Img[4]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[4]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, Img[5]->sizeX, Img[5]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Img[5]->data);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	
	if (distance == 0) elevation = 0;
	else
	{
		elevation = acos(pz / distance) * 180 / PI;
	}
	if (px == 0) {
		azumuth = 0;
	}
	else
		azumuth = atan(py / px) * 180 / PI;
	azumuth = 270;
	if (cameradistance == 0) cameraelevation = 0;
	else
	{
		cameraelevation = acos(camerapz / cameradistance) * 180 / PI;
	}
	if (camerapx == 0) {
		cameraazumuth = 90;
	}
	else
		cameraazumuth = atan(camerapy / camerapx) * 180 / PI;


	dro.y.x = 1, dro.y.y = 0, dro.y.z = 0;
	dro.z.x = 0, dro.z.y = 1, dro.z.z = 0;
	dro.x.x = 0, dro.x.y = 0, dro.x.z = 1;
	dro.drone.x = 0, dro.drone.y = 0, dro.drone.z = 0;

}

double Determinant(GLfloat **a, int n) {
	int i, j, j1, j2;  double det = 0;  GLfloat **m = NULL;

	if (n == 2) { det = a[0][0] * a[1][1] - a[1][0] * a[0][1]; }
	else {
		det = 0;
		for (j1 = 0;j1<n;j1++) {
			m = new GLfloat*[n - 1];

			for (i = 0;i<n - 1;i++) {
				m[i] = new GLfloat[n - 1];
			}

			for (i = 1;i<n;i++) {
				j2 = 0;
				for (j = 0;j<n;j++) {
					if (j == j1) continue;
					m[i - 1][j2] = a[i][j];
					j2++;
				}
			}

			det += pow(-1.0, j1 + 2.0) * a[0][j1] * Determinant(m, n - 1);
			for (i = 0;i<n - 1;i++) delete[] m[i];
			delete[] m;
		}
	}

	return(det);
}// 4차원 행렬을 구하므로 n=4 를 대입

void CoFactor(GLfloat **a, int n, GLfloat **b)
{
	int i, j, i2, j2, i1, j1;
	GLfloat det;
	GLfloat **m;
	m = new GLfloat*[n - 1]; for (i = 0;i < n - 1;i++) { m[i] = new GLfloat[n - 1]; }



	for (j = 0;j<n;j++) {
		for (i = 0;i<n;i++) {

			/* Form the adjoint a_ij */
			i1 = 0;
			for (i2 = 0;i2<n;i2++) {
				if (i2 == i)
					continue;
				j1 = 0;
				for (j2 = 0;j2<n;j2++) {
					if (j2 == j)
						continue;
					m[i1][j1] = a[i2][j2];
					j1++;
				}
				i1++;
			}

			det = Determinant(m, n - 1);

			b[i][j] = pow(-1.0, i + j + 2.0) * det;
		}
	}
	for (i = 0;i < n - 1;i++) delete[] m[i];
	delete[] m;
}
void Transpose(GLfloat **b, int n) {
	int i, j; GLfloat tmp;
	for (i = 0; i < n;i++) {
		for (j = i + 1;j < n;j++) {
			tmp = b[i][j];
			b[i][j] = b[j][i];
			b[j][i] = tmp;
		}
	}
}
void div_det(GLfloat **b, int n, double det) {
	int i, j;
	for (i = 0; i < n;i++) {
		for (j = 0;j < n;j++) {
			b[i][j] = b[i][j] / det;
		}
	}
}
void gluInvertMatrix(GLfloat **a, GLfloat **b) {

	//시선행렬의 역행렬을 구해준다.  

	double det = Determinant(a, 4);	//행렬식을 구함

	CoFactor(a, 4, b);  //여인수(cofactor 구하는 함수(직접구현)

	Transpose(b, 4);  //전치행렬 구하는 함수(직접 구현)

	div_det(b, 4, det); //역행렬 구하는 함수(직접 구현)

}


void draw_sp(void) {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);

	glutSolidSphere(50.0f, 30, 30);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
}
void draw_sky(void) {
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	float g_nSkySize = 1024.0f;

	//앞면
	glBegin(GL_QUADS);
	glTexCoord3d(1.0, -1.0, -1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glEnd();

	//뒷면
	glBegin(GL_QUADS);
	glTexCoord3d(-1.0, -1.0, -1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glEnd();

	//오른쪽
	glBegin(GL_QUADS);
	glTexCoord3d(1.0, 1.0, 1.0);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glEnd();

	//왼쪽
	glBegin(GL_QUADS);
	glTexCoord3d(1.0, -1.0, 1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glEnd();

	//윗면
	glBegin(GL_QUADS);
	glTexCoord3d(-1.0, -1.0, 1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0);
	glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glEnd();

	//아랫면
	glBegin(GL_QUADS);
	glTexCoord3d(-1.0, -1.0, -1.0);
	glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0);
	glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0);
	glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0);
	glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glEnd();
}

void draw_box() {
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glEnd();


	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glEnd();
}


void draw_bigdisp() {
	glViewport(0, 0, 720, 720);

	switch (sight)
	{
	case 0:
		gluLookAt(eyex + dro.drone.x, eyey + dro.drone.y, eyez + dro.drone.z, dro.drone.x, dro.drone.y, dro.drone.z, 0, 0, upy);
		break;
	case 1:
		gluLookAt(dro.drone.x + tmp + tmp2, dro.drone.y + 9.3 - (spinf / 8.547) + tmp1 + tmp3, dro.drone.z + 7.375 + (spinf / 6.61), cameraeyex + dro.drone.x, cameraeyey + dro.drone.y + 7.3, cameraeyez + dro.drone.z + 7.375, 0, 0, cameraupy);

		break;
	case 2:
		gluLookAt(0, -190, 80, dro.drone.x, dro.drone.y, dro.drone.z + 10, 0, 0, 1);
		break;
	}
	GLfloat light_position[] = { 100.0f, 100.0f, 1024.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	
	glPushMatrix();
	{
		glTranslatef(dro.drone.x, dro.drone.y, dro.drone.z);
		glRotatef(move_spin, 0, 0, 1);

		GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		glEnable(GL_LIGHT2);

		GLfloat light_position2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		if (sight != 1) {
		draw_d();
		}
		emission[0] = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

	}
	glPopMatrix();
	

	glPushMatrix();
	{
		glTranslatef(30, 60, 3);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.8f, 0.8f, 0.0f);
		glutWireTeapot(5.0f);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, 30, 10);
		glColor3f(0.3f, 0.8f, 0.1f);
		gluSphere(m_pQuadric, 10.0f, 30, 30);

	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -150, -2.5);
		glPushMatrix();
		{

			glTranslatef(0, -40, 70);
			glScalef(0.9, 0.9, 1);
			glColor3f(0.9f, 0.5f, 0.3f);
			glutSolidSphere(10, 50, 50);

		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, -40, 50);
			glScalef(1, 0.8, 1.2);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(15, -40, 45);
			glScalef(0.5, 0.5, 1.4);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-15, -40, 45);
			glScalef(0.5, 0.5, 1.4);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(6, -40, 20);
			glScalef(0.5, 0.5, 1.8);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-6, -40, 20);
			glScalef(0.5, 0.5, 1.8);
			draw_box();
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 1024);
	glRotatef(90, 1, 0, 0);
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glEnable(GL_LIGHT1);
	GLfloat light_position1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	draw_sky();
	emission[0] = 0.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMatrixMode(GL_TEXTURE);  glPushMatrix();  //현재의 texture 상태를 저장

	GLfloat camera[16];  GLfloat inv_camera[16];  glGetFloatv(GL_MODELVIEW_MATRIX, camera);  camera[14] = 0;

	GLfloat ** camera_4x4, **inv_camera_4x4;  camera_4x4 = new GLfloat*[4];  inv_camera_4x4 = new GLfloat*[4];  for (int i = 0; i<4; i++) { camera_4x4[i] = new GLfloat[4];   inv_camera_4x4[i] = new GLfloat[4]; }

	for (int row = 0; row<4; row++) { for (int col = 0; col<4; col++) { camera_4x4[row][col] = camera[row * 4 + col]; } }


	gluInvertMatrix(camera_4x4, inv_camera_4x4);

	for (int row = 0; row<4; row++) { for (int col = 0; col<4; col++) { inv_camera[row * 4 + col] = inv_camera_4x4[row][col]; } }


	glMultMatrixf(inv_camera);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glColor3f(1, 1, 1);
	draw_sp();
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);  glPopMatrix();  //역행렬 곱해주기 전의 상태로 돌아간다.

	glMatrixMode(GL_MODELVIEW);  //모델 뷰 상태로 되돌려 놓음
	glPopMatrix();

	glFlush();
}
void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	switch (eye)
	{
	case 1:
		azumuth--;
		eye = 0;
		break;
	case 2:
		azumuth++;
		eye = 0;
		break;
	case 3:
		elevation--;
		eye = 0;
		break;
	case 4:
		elevation++;
		eye = 0;
		break;
	case 5:
		if (spin_w == false) spin_w = true;
		else {
			spin_w = false;
			spin_l = false;
		}
		eye = 0;
		break;
	case 6:
		if (distance <= 1) distance = 1;
		else {
			distance--;
		}
		eye = 0;
		break;
	case 7:
		distance++;
		eye = 0;
		break;

	}

	if (wing_speed > 100) {
		switch (move)
		{
		case 9:
			cameraelevation--;
			rotate_cam++;
			break;
		case 10:
			cameraelevation++;
			rotate_cam--;
			break;
		case 11:
			cameraazumuth++;
			rotate_cam1++;
			if (rotate_cam1 <270 && rotate_cam1>90) tmp2 += 0.04;
			else if (rotate_cam1 <90 && rotate_cam1>0 || rotate_cam1 < 360 && rotate_cam1 > 270) tmp2 -= 0.04;
			if (rotate_cam1 <360 && rotate_cam1>180) tmp3 += 0.04;
			else if (rotate_cam1 <180 && rotate_cam1>0) tmp3 -= 0.04;
			break;
		case 12:
			cameraazumuth--;
			rotate_cam1--;
			if (rotate_cam1 <270 && rotate_cam1>90) tmp2 -= 0.04;
			else if (rotate_cam1 <90 && rotate_cam1>0 || rotate_cam1 < 360 && rotate_cam1 > 270) tmp2 += 0.04;
			if (rotate_cam1 <360 && rotate_cam1>180) tmp3 -= 0.04;
			else if (rotate_cam1 <180 && rotate_cam1>0) tmp3 += 0.04;
			break;

		case 13:
			if (spin_l == false) spin_l = true;
			else spin_l = false;
			break;
		case 0:
			moving_f = false; moving_b = false;  moving_l = false; moving_r = false;
			break;

		}
		move = 0;
	}
	El = elevation * PI / 180;
	Az = azumuth * PI / 180;



	eyex = distance * sin(El) * cos(Az);
	eyey = distance * sin(El) * sin(Az);
	eyez = distance * cos(El);

	if (azumuth > 360) {
		azumuth = azumuth - 360;
	}
	else if (azumuth < -360) {
		azumuth = azumuth + 360;
	}

	if (elevation > 360) {
		elevation = elevation - 360;
	}
	else if (elevation < 0) {
		elevation = elevation + 360;
	}

	upy = distance * cos((elevation - 90) * PI / 180);
	cameraEl = cameraelevation * PI / 180;
	cameraAz = cameraazumuth * PI / 180;


	cameraeyex = distance * sin(cameraEl) * cos(cameraAz);
	cameraeyey = distance * sin(cameraEl) * sin(cameraAz);
	cameraeyez = distance * cos(cameraEl);

	if (rotate_cam > 48) rotate_cam = 48;
	else if (rotate_cam < -87) rotate_cam = -87;
	if (rotate_cam1 > 360) rotate_cam1 -= 360;
	else if (rotate_cam1 < 0) rotate_cam1 += 360;

	if (cameraazumuth > 0) {
		cameraazumuth = cameraazumuth - 360;
	}
	else if (cameraazumuth < -360) {
		cameraazumuth = cameraazumuth + 360;
	}

	if (cameraelevation > 174) {
		cameraelevation = 174;
	}
	else if (cameraelevation < 39) {
		cameraelevation = 39;
	}
	if (move_spin < 0) move_spin += 360;
	else if (move_spin > 360) move_spin -= 360;
	cameraupy = cameradistance * cos((cameraelevation - 90) * PI / 180);
	
	
	draw_bigdisp();
	draw_text();
	draw_disp();

	glutPostRedisplay();
	glutSwapBuffers(); //Back buffer를 front buffer로 바꿈


}



void sub_menu_function(int option) {
	printf("Submenu %d has been selected\n", option);
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
}

void idle(void) {
	if (wing_speed > 100) {
		if (keyPressed['w']) {
			if (moving_f) moving_f = true;
			else moving_f = false;
			moving_b = false;  moving_l = false; moving_r = false;
			spinf -= 2;
			if (spinf < -30) spinf = -30;
		}
		if (keyPressed['s']) {
			if (moving_b) moving_b = true;
			else moving_b = false;
			spinf += 2;
			if (spinf > 30) spinf = 30;
		}
		if (keyPressed['a']) {
			if (moving_l) moving_l = true;
			else moving_l = false;
			spinl -= 2;
			
			if (spinl < -30) spinl = -30;
		}
		if (keyPressed['d']) {
			if (moving_r) moving_r = true;
			else moving_r = false;
			spinl += 2;
			
			if (spinl > 30) spinl = 30;
		}
		if (keyPressed['r']) {
			dro.trans(0, 0, 2);
			move_up += 2;

		}
		if (keyPressed['f']) {

			if (dro.drone.z > 0) {
				dro.trans(0, 0, -2);
				move_up -= 2;
			}
		}
		if (keyPressed['q']) {
			cameraazumuth++;
			move_spin++;
			azumuth++;
			dro.rota(1);
			if (move_spin <270 && move_spin>90) tmp += 0.103333;
			else if (move_spin <90 && move_spin>0 || move_spin < 360 && move_spin > 270) tmp -= 0.103333;
			if (move_spin <360 && move_spin>180) tmp1 += 0.103333;
			else if (move_spin <180 && move_spin>0) tmp1 -= 0.103333;
		}
		
		if (keyPressed['e']) {
			cameraazumuth--;
			move_spin--;
			azumuth--;
			dro.rota(-1);
			if (move_spin <270 && move_spin>90) tmp -= 0.103333;
			else if (move_spin <90 && move_spin>0 || move_spin < 360 && move_spin > 270) tmp += 0.103333;
			if (move_spin <360 && move_spin>180) tmp1 -= 0.103333;
			else if (move_spin <180 && move_spin>0) tmp1 += 0.103333;
		}
	}
	if (keyPressed['z']) {
		if(wing_speed < 140) wing_speed++;
	}
	if (keyPressed['x']) {
		wing_speed--;
	}
	if (moving_f == false && spinf < 0) {
		spinf = spinf + 0.4;
		if (spinf > 0) spinf = 0;
	}
	if (moving_b == false && spinf > 0) {
		spinf = spinf - 0.4;
		if (spinf < 0) spinf = 0;
	}
	if (moving_r == false && spinl > 0) {
		spinl = spinl - 0.4;
		if (spinl < 0) spinl = 0;
	}
	if (moving_l == false && spinl < 0) {
		spinl = spinl + 0.4;
		if (spinl > 0) spinl = 0;
	}
	if (spinf < 0 && spinf >-30) dro.trans(0, -(spinf*0.1 - (wing_speed - 110)*0.08), 0);
	if (spinf > 0 && spinf < 30) dro.trans(0, -(spinf*0.1 + (wing_speed - 110)*0.08), 0);
	if (spinl > 0 && spinl < 30) dro.trans((spinl*0.1 + (wing_speed - 110)*0.08), 0, 0);
	if (spinl < 0 && spinl >-30) dro.trans((spinl*0.1 - (wing_speed - 110)*0.08), 0, 0);

	if (spin_l == true) {
		if (legspin > -60) {
			legspin = legspin - 2;
			if (legspin < 0 && legspin > -30) {
				if (r_legX < 11.081) r_legX += 0.07;
				if (l_legX > -11.151) l_legX -= 0.08;
				if (r_wingX < 11.342) r_wingX += 0.07;
				if (l_wingX > -11.342) l_wingX -= 0.07;
			}
			else if (legspin <= -30 && legspin > -60) {
				if (r_legX > 10.138) r_legX -= 0.07;
				if (l_legX < -10.138) l_legX += 0.08;
				if (r_wingX > 10.149) r_wingX -= 0.07;
				if (l_wingX < -10.149) l_wingX += 0.07;
			}
		}
		if (wingZ < 16.6) wingZ += 0.335;
		if (legZ < 11) legZ += 0.335;

	}
	else {
		if (legspin < 0) {
			legspin = legspin + 2;
			if (legspin > -30 && legspin <= 0) {
				if (r_legX > 10.138) r_legX -= 0.07;
				if (l_legX < -10.138) l_legX += 0.08;
				if (r_wingX > 10.149) r_wingX -= 0.07;
				if (l_wingX < -10.149) l_wingX += 0.07;
			}
			else if (legspin >= -60 && legspin < -30) {
				if (r_legX < 11.081) r_legX += 0.07;
				if (l_legX > -11.151) l_legX -= 0.08;
				if (r_wingX < 11.342) r_wingX += 0.07;
				if (l_wingX > -11.342) l_wingX -= 0.07;
			}
		}
		if (wingZ > 6.737) wingZ -= 0.335;
		if (legZ > 1.119) legZ -= 0.335;
	}
	
	if (spin_w == true) {
		if (wing_speed < 110) {
			wing_speed++;
		}
		wingspin = wingspin + wing_speed;
		if (wing_speed > 100 && wing_speed < 110) {
			if (dro.drone.z < 26) {
				dro.trans(0, 0, 2);
			}
		}
		if (wingspin > 360) wingspin -= 360;
		
	}

	else {
		if (dro.drone.z < 30) {
			if (wing_speed > 0) wing_speed -= 0.5;
			wingspin = wingspin + wing_speed;
			if (wing_speed < 100) {
				if (dro.drone.z > 0) {
					dro.trans(0, 0, -1);
				}
			}
			if (wingspin > 360) wingspin -= 360;
		}
		else
		{
			if (wing_speed > 100) wing_speed -= 0.5;
			wingspin = wingspin + wing_speed;
			if (wingspin > 360) wingspin -= 360;
			dro.trans(0, 0, -1);
		}
	}
	glutPostRedisplay();
}

void file_load(char *fname) {
	i = 0; j = 0; k = 0; l = 0;

	FILE * fp;
	int read;
	int read1;
	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}

	while (!(feof(fp)))
	{
		read = fscanf(fp, "%s", &ch);
		if (!strcmp("v", ch))
		{
			fscanf(fp, "%f %f %f\n", &x, &y, &z);
			i++;
			v_x[d_num][i] = x;
			v_y[d_num][i] = y;
			v_z[d_num][i] = z;

		}
		else if (!strcmp(ch, "vt")) {
			fscanf(fp, "%f %f\n", &u, &v);
			l++;
			vtu[d_num][l] = u;
			vtv[d_num][l] = v;
		}
		else if (!strcmp("vn", ch))
		{
			fscanf(fp, "%f %f %f\n", &x, &y, &z);
			k++;
			nvx[d_num][k] = x;
			nvy[d_num][k] = y;
			nvz[d_num][k] = z;
		}
		if (!strcmp("f", ch))
		{
			int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			j++;
			f_v1[d_num][j] = v1;
			f_v2[d_num][j] = v2;
			f_v3[d_num][j] = v3;
			f_vt1[d_num][j] = vt1;
			f_vt2[d_num][j] = vt2;
			f_vt3[d_num][j] = vt3;
			f_vn1[d_num][j] = vn1;
			f_vn2[d_num][j] = vn2;
			f_vn3[d_num][j] = vn3;
			f_num = j;
		}
	}

	fclose(fp);
}

void specialkeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		eye = 1;
		break;
	case GLUT_KEY_RIGHT:
		eye = 2;
		break;
	case GLUT_KEY_UP:
		eye = 3;
		break;
	case GLUT_KEY_DOWN:
		eye = 4;
		break;
	}
	glutPostRedisplay();

}


void drow_tri() {
	glBegin(GL_TRIANGLES);
	for (int n = 0; n <= f_num; n++) {
		glTexCoord2f(vtu[d_num][f_vt1[d_num][n]], vtv[d_num][f_vt1[d_num][n]]);
		glNormal3f(nvx[d_num][f_vn1[d_num][n]], nvy[d_num][f_vn1[d_num][n]], nvz[d_num][f_vn1[d_num][n]]);
		glVertex3f(v_x[d_num][f_v1[d_num][n]], v_y[d_num][f_v1[d_num][n]], v_z[d_num][f_v1[d_num][n]]);

		glTexCoord2f(vtu[d_num][f_vt2[d_num][n]], vtv[d_num][f_vt2[d_num][n]]);
		glNormal3f(nvx[d_num][f_vn2[d_num][n]], nvy[d_num][f_vn2[d_num][n]], nvz[d_num][f_vn2[d_num][n]]);
		glVertex3f(v_x[d_num][f_v2[d_num][n]], v_y[d_num][f_v2[d_num][n]], v_z[d_num][f_v2[d_num][n]]);

		glTexCoord2f(vtu[d_num][f_vt3[d_num][n]], vtv[d_num][f_vt3[d_num][n]]);
		glNormal3f(nvx[d_num][f_vn3[d_num][n]], nvy[d_num][f_vn3[d_num][n]], nvz[d_num][f_vn3[d_num][n]]);
		glVertex3f(v_x[d_num][f_v3[d_num][n]], v_y[d_num][f_v3[d_num][n]], v_z[d_num][f_v3[d_num][n]]);
	}
	glEnd();

}

void draw_d(void) {
	//glTranslatef(0, 1, 0);
	glRotatef(spinf, 1, 0, 0);
	glRotatef(spinl, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0, 0, 5.2);
	//d_num = 0;
	//file_load("bodynew.obj");
	glPushMatrix();
	d_num = 0;
	glColor3f(0.65, 0.65, 0.65);
	drow_tri();
	glPopMatrix();

	//d_num = 9;
	//file_load("bat.obj");
	glPushMatrix();
	d_num = 9;
	glColor3f(0.1, 0.1, 0.1);
	drow_tri();
	glPopMatrix();

	//d_num = 10;
	//file_load("bodyc.obj");
	glPushMatrix();
	d_num = 10;
	glColor3f(0.1, 0.1, 0.1);
	drow_tri();
	glPopMatrix();

	glPushMatrix();
	d_num = 1;	//d_num = 1;
				//file_load("wing.obj");
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(l_wingX, -11.568, wingZ);
	glRotatef(move__, 1, 0, 0);
	glRotatef(wingspin, 0, 0, -1);

	drow_tri();
	glPopMatrix();


	glPushMatrix();
	d_num = 1;
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(r_wingX, -11.568, wingZ);
	glRotatef(move__, 1, 0, 0);

	glRotatef(wingspin, 0, 0, -1);
	drow_tri();
	glPopMatrix();


	glPushMatrix();
	d_num = 1;
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(l_wingX, 11.323, wingZ);
	glRotatef(move__, 1, 0, 0);
	glRotatef(wingspin, 0, 0, -1);
	drow_tri();
	glPopMatrix();

	glPushMatrix();
	d_num = 1;
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(r_wingX, 11.323, wingZ);
	glRotatef(move__, 1, 0, 0);
	glRotatef(wingspin, 0, 0, -1);
	drow_tri();
	glPopMatrix();

	//d_num = 2;
	//file_load("l_leg0.obj");
	glPushMatrix();
	d_num = 2;
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(-1.597, 0.501, 9.789);
	glRotatef(-30, 0, 1, 0);
	glRotatef(-legspin, 0, 1, 0);
	drow_tri();
	glPopMatrix();

	//d_num = 3;
	//file_load("r_leg0.obj");
	glPushMatrix();
	d_num = 3;
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(1.271, 0.475, 9.789);
	glRotatef(30, 0, 1, 0);
	glRotatef(legspin, 0, 1, 0);
	drow_tri();
	glPopMatrix();

	//d_num = 4;
	//file_load("l_leg1.obj");
	glPushMatrix();
	d_num = 4;
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(l_legX, -0.122, legZ);
	drow_tri();
	glPopMatrix();

	//d_num = 5;
	//file_load("r_leg1.obj");
	glPushMatrix();
	d_num = 5;
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(r_legX, -0.122, legZ);
	drow_tri();
	glPopMatrix();

	//d_num = 6;
	//file_load("camera.obj");
	glPushMatrix();
	d_num = 6;
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(0, 5.155, 2.199);

	glRotatef(rotate_cam1, 0, 0, 1);
	glRotatef(rotate_cam, 1, 0, 0);
	

	drow_tri();
	glPopMatrix();



	//d_num = 7;
	//file_load("Cylin.obj");
	glPushMatrix();
	d_num = 7;
	glColor3f(0.2, 0.2, 0.2);

	drow_tri();
	glPopMatrix();

	//d_num = 8;
	//file_load("cam_l.obj");
	glPushMatrix();
	d_num = 8;
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(0, 5.155, 3.584);
	glRotatef(rotate_cam1, 0, 0, 1);
	drow_tri();
	glPopMatrix();
	
	
	glPopMatrix();

}


void keyboard(unsigned char key, int x, int y) {
	keyPressed[key] = true;
	switch (key)
	{
	case 'q':
		move = 5;
		break;
	case 'e':
		move = 6;
		break;
	case 'y':
		move = 9;
		break;
	case 'h':
		move = 10;
		break;
	case 'g':
		move = 11;
		break;
	case 'j':
		move = 12;
		break;
	case 'l':
		move = 13;
		break;
	case '1':
		eye = 5;
		break;
	case '2':
		sight = 0;
		break;
	case '3':
		sight = 1;
		break;
	case '4':
		sight = 2;
		break;
	case 'i':
		eye = 6;
		break;
	case 'o':
		eye = 7;
		break;
	default:
		break;
	}
	glutPostRedisplay();

}
void myKeyboardUp(unsigned char key, int x, int y) {
	keyPressed[key] = false;
}

void draw_string(void *font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}
void draw_text()
{
	glViewport(720, 180, 178, 520);
	glLoadIdentity();

	if(sight == 0)
	draw_string(GLUT_BITMAP_HELVETICA_18, "world", -4, 4.8, 1.0, 1.0, 1.0);
	if (sight == 1)
	draw_string(GLUT_BITMAP_HELVETICA_18, "drone camera", -4, 4.8, 1.0, 1.0, 1.0);
	if (sight == 2)
	draw_string(GLUT_BITMAP_HELVETICA_18, "person eye", -4, 4.8, 1.0, 1.0, 1.0);

	draw_string(GLUT_BITMAP_HELVETICA_18, "world : 2", -4, 4.4, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "drone camera : 3", -4, 4.0, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "person eye : 4", -4,  3.6, 1.0, 1.0, 1.0);

	draw_string(GLUT_BITMAP_HELVETICA_18, "move front : w", -4, 3.0, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "move back : s", -4, 2.6, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "move right : d", -4, 2.2, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "move left : a", -4, 1.8, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "move up : r", -4, 1.4, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "move down : f", -4, 1.0, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "rotate right : e", -4, 0.6, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "rotate left : q", -4, 0.2, 1.0, 1.0, 1.0);


	draw_string(GLUT_BITMAP_HELVETICA_18, "camera up : y", -4, -0.2, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "camera down : h", -4, -0.6, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "camera right : j", -4, -1.0, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "camera left : g", -4, -1.4, 1.0, 1.0, 1.0);

	draw_string(GLUT_BITMAP_HELVETICA_18, "speed up : z", -4, -2.2, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "speed down : x", -4, -2.6, 1.0, 1.0, 1.0);

	draw_string(GLUT_BITMAP_HELVETICA_18, "drone leg up : l", -4, -3.0, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "drone start : 1", -4, -3.4, 1.0, 1.0, 1.0);
	
	glFlush();

}
void draw_disp(void) {
	glViewport(722, 2, 176, 176);
	gluLookAt(dro.drone.x + tmp + tmp2, dro.drone.y + 9.3 - (spinf / 8.547) + tmp1 + tmp3, dro.drone.z + 7.375 + (spinf / 6.61), cameraeyex + dro.drone.x, cameraeyey + dro.drone.y + 7.3, cameraeyez + dro.drone.z + 7.375, 0, 0, cameraupy);

	glPushMatrix();
	GLfloat light_position[] = { 100.0f, 100.0f, 1024.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glPushMatrix();
	glTranslatef(dro.drone.x, dro.drone.y, dro.drone.z);
	glRotatef(move_spin, 0, 0, 1);
	GLfloat light_position2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(30, 60, 3);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.8f, 0.8f, 0.0f);
		glutWireTeapot(5.0f);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, 30, 10);
		glColor3f(0.3f, 0.8f, 0.1f);
		gluSphere(m_pQuadric, 10.0f, 30, 30);

	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -150, -2.5);
		glPushMatrix();
		{

			glTranslatef(0, -40, 70);
			glScalef(0.9, 0.9, 1);
			glColor3f(0.9f, 0.5f, 0.3f);
			glutSolidSphere(10, 50, 50);

		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, -40, 50);
			glScalef(1, 0.8, 1.2);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(15, -40, 45);
			glScalef(0.5, 0.5, 1.4);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-15, -40, 45);
			glScalef(0.5, 0.5, 1.4);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(6, -40, 20);
			glScalef(0.5, 0.5, 1.8);
			draw_box();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-6, -40, 20);
			glScalef(0.5, 0.5, 1.8);
			draw_box();
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 1024);
	glRotatef(90, 1, 0, 0);
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glEnable(GL_LIGHT1);
	GLfloat light_position1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	draw_sky();
	emission[0] = 0.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMatrixMode(GL_TEXTURE);  glPushMatrix();  //현재의 texture 상태를 저장

	GLfloat camera[16];  GLfloat inv_camera[16];  glGetFloatv(GL_MODELVIEW_MATRIX, camera);  camera[14] = 0;

	GLfloat ** camera_4x4, **inv_camera_4x4;  camera_4x4 = new GLfloat*[4];  inv_camera_4x4 = new GLfloat*[4];  for (int i = 0; i<4; i++) { camera_4x4[i] = new GLfloat[4];   inv_camera_4x4[i] = new GLfloat[4]; }

	for (int row = 0; row<4; row++) { for (int col = 0; col<4; col++) { camera_4x4[row][col] = camera[row * 4 + col]; } }


	gluInvertMatrix(camera_4x4, inv_camera_4x4);

	for (int row = 0; row<4; row++) { for (int col = 0; col<4; col++) { inv_camera[row * 4 + col] = inv_camera_4x4[row][col]; } }


	glMultMatrixf(inv_camera);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glColor3f(1, 1, 1);
	draw_sp();
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);  glPopMatrix();  //역행렬 곱해주기 전의 상태로 돌아간다.

	glMatrixMode(GL_MODELVIEW);  //모델 뷰 상태로 되돌려 놓음
	glPopMatrix();
	glPopMatrix();

	glFlush();
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseEyetrans = true;
	}
	else mouseEyetrans = false;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		mouseScale = true;
	}
	else mouseScale = false;
}
void motion(int x, int y) {
	if (mouseEyetrans) {
		if (x > moux) {
			azumuth -= 2;
			moux = x;
		}
		else if (x < moux) {
			azumuth += 2;
			moux = x;
		}
		else if (y > mouy) {
			elevation-= 2;
			mouy = y;
		}
		else if (y < mouy) {
			elevation+= 2;
			mouy = y;
		}
	}
	if (mouseScale) {
		if (y > scale) {
			distance += 2;
			scale = y;
		}

		else if (y < scale) {
			if (distance > 0) {
				distance -= 2;
				scale = y;
			}
		}
	}

}