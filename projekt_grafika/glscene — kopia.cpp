#include <windows.h> /*naglowek dla windowsa*/

#include <gl/gl.h>/*naglowek dla opengl32*/
#include <gl/glu.h>/*naglowek dla glu32*/
//#include <GL/glext.h>
#include <gl/freeglut.h> /*naglowek dla freeglut'a*/
#include <gl/glext.h> //naglowek do gluBuild2DMipmaps

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <string>
#include <fstream>
#include <sstream>

#include "OBJ.h"
#include "Vertex.h"

#include "glscene.h"
/*OKNO*/

GLint glScene::winx_ = 0;
GLint glScene::winy_ = 0;

GLfloat przemieszczenie = 0.0f; //ZMIENNA PRZEMIESZCZENIA
GLfloat kat = 0.0f; //ZMIENNA KAT

// obiekty potrzebne do załadowania tekstury
Tex MOD[4];
GLuint IDtext[4];

// wektory światła
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position[] = { 0.0f, 50.0f, 50.0f, 1.0f };
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLfloat angle = 0.0f;
GLfloat lx=0.0f,lz=-1.0f;
GLfloat x=0.0f,y=10.0f,z=5.0f;

OBJ ludzik("ludzik_n.obj");
OBJ gun("gun_n.obj");
OBJ domek("dom_n.obj");

void drawObject(OBJ, float, GLuint);

GLUquadricObj *Snieg = gluNewQuadric();

bool fire = false;
bool one = true;
bool resetTimer = false;

float time = 1;
float lot_kuli = 10000.0f;

float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;
float distance = 0.0f;

Vertex * currentPosition;

GLfloat ludzik_position[16];
GLfloat pocisk_position[16];

bool ludzik_dead = false;

GLfloat zasieg_pocisku = 750.0f;

glScene::glScene()
:
	repaintRequest_(GL_FALSE)
{
	rot_[0] = rot_[1] = rot_[2] = 0.0f;

}

glScene::~glScene()
{
}

void glScene::Resize()
{
   Resize(winx_, winy_);
}

void glScene::Resize(int _w, int _h)
{
	winx_ = _w;
	winy_ = _h;

	if(_h == 0)
   		_h = 1;
	if(_w == 0)
   		_w = 1;/*nie dziel przez zero*/

	//ustawienie viewportu
	glViewport(0, 0, _w, _h);

	//macierze projekcji potrzebna do perspektywy
	glMatrixMode(GL_PROJECTION);
	//ustaw aktualna macierz na I (resetuje macierz do pierwotnego stanu)
	glLoadIdentity();

	//ustaw uklad wspolrzednych
	//glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	//perspektywa
	gluPerspective(70.0,(double)_w/(double)_h, 0.1, 1000.0);

	//macierze modelowania transformacje oddziąłują na macierz modeli
	glMatrixMode(GL_MODELVIEW);
	//ustaw aktualna macierz na I
	glLoadIdentity();
}



void glScene::SetupRC()
{
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);

   // światło

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // tekstury
	IDtext[0] = MOD[0].loadBMP("303.bmp");		//załadowanie pliku (z folderu z projektem)
	glGenTextures(1, &IDtext[0]);		//generowanie tekstury
	glBindTexture(GL_TEXTURE_2D, IDtext[0]);	//ustawienie tej tekstury (nią będą kolorowane przedmioty)
	//pięć poniższych nie jest wymagane
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, MOD[0].width, MOD[0].height, GL_BGR_EXT, GL_UNSIGNED_BYTE, MOD[0].data);
	delete[] MOD[0].data;

	IDtext[1] = MOD[1].loadBMP("gun.bmp");		//załadowanie pliku (z folderu z projektem)
	glGenTextures(1, &IDtext[1]);		//generowanie tekstury
	glBindTexture(GL_TEXTURE_2D, IDtext[1]);	//ustawienie tej tekstury (nią będą kolorowane przedmioty)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, MOD[1].width, MOD[1].height, GL_BGR_EXT, GL_UNSIGNED_BYTE, MOD[1].data);
	delete[] MOD[1].data;

	IDtext[2] = MOD[2].loadBMP("dom.bmp");		//załadowanie pliku (z folderu z projektem)
	glGenTextures(1, &IDtext[2]);		//generowanie tekstury
	glBindTexture(GL_TEXTURE_2D, IDtext[2]);	//ustawienie tej tekstury (nią będą kolorowane przedmioty)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, MOD[2].width, MOD[2].height, GL_BGR_EXT, GL_UNSIGNED_BYTE, MOD[2].data);
	delete[] MOD[2].data;

    glLoadIdentity();
}

void glScene::CleanRC()
{
}

void glScene::RenderScene()
{
    glLoadIdentity();
    //GLUquadricObj *Ksiezyc = gluNewQuadric();
    //GLUquadricObj *Wszechswiat = gluNewQuadric();

    gluLookAt(x, 1.0f,z, x+lx, 1.0f, z+ lz, 0.0f, 1.0f, 0.0f);

	//std::cout << "Twoja pozycja:" << x << ", 1.0, " << z << "\n";

	//malujemy scene, mozna skasowac flage aby scena ciagle sie nie odmalowywala
	repaintRequest_ = GL_FALSE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//czyszcenie okna

	glPushMatrix(); // glowna macierz

	glTranslated(0.0f,-30.0f,0.0f); //cofniecie perspektywy do tyłu
	glRotatef(-90,1.0f,0.0f,0.0f);

    glBegin(GL_QUADS);
	
	glPointSize(0.1f);

	//trawa
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-46.0f,150.0f ,-0.02f);
	glVertex3f(-46.0f,-150.0f,-0.02f);
	glVertex3f(46.0f,-150.0f,-0.02f);
	glVertex3f(46.0f,150.0f,-0.02f);

	//chodnik
	glColor3f(0.0f,0.2f,0.1f);
	glVertex3f(-6.0f,150.0f,-0.01f);
	glVertex3f(-6.0f,-150.0f,-0.01f);
	glVertex3f(6.0f,-150.0f,-0.01f);
	glVertex3f(6.0f,150.0f,-0.01f);

	//jezdnia
	glColor3f(0.0f,0.0f,0.0f);
	glVertex3f(-4.0f,150.0f,0.0f);
	glVertex3f(-4.0f,-150.0f,0.0f);
	glVertex3f(4.0f,-150.0f,0.0f);
	glVertex3f(4.0f,150.0f,0.0f);

    glEnd();

    //1 budynek
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,0.0f);
    glScalef(30.0f,40.0f,50.0f);
    glTranslatef(-0.5f,-2.5f,0.25f);

    glutSolidCube(0.5f);
    glPopMatrix();

    //2 budynek
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,0.0f);
    glScalef(40.0f,60.0f,40.0f);
    glTranslatef(0.5f,2.0f,0.25f);
    glutSolidCube(0.5f);
    glPopMatrix();

    glPushMatrix(); // balwan

        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f,3.0f,-50.0f);
        //gluSphere(Snieg, 5.0, 50, 50);

        glTranslatef(0.0f,7.0f,0.0f);
        //gluSphere(Snieg, 4.0, 50, 50);

        glTranslatef(0.0f,5.0f,0.0f);
        //gluSphere(Snieg, 3.0, 50, 50);

    glPopMatrix(); // !balwan

	glPushMatrix(); // LUDZIK
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 80.0f, 3.0f);
		if (ludzik_dead)
		{
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		}
		drawObject(ludzik, 1.0f, IDtext[0]);

		glGetFloatv(GL_MODELVIEW_MATRIX, ludzik_position);
		//std::cout << "Pozycja ludzik: " << ludzik_position[12] << " " << ludzik_position[13] << " " << ludzik_position[14] << "\n";
	
	glPopMatrix(); // !LUDZIK

	glPushMatrix();	// BRON
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(x, -z, 0.0f);
		glRotatef(95.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-angle * 57.2957795, 0.0f, 0.0f, 1.0f); // Rotate to correspond to the camera
		glTranslatef(30.0f, 15.0f, 20.0f);
		drawObject(gun, 100.0f, IDtext[1]);
	glPopMatrix(); // !BRON

	


	glPushMatrix(); // DOMEK
		glTranslatef(200.0f, 0.0f, 0.0f);
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		drawObject(domek, 20.0f, IDtext[2]);
	glPopMatrix(); // !DOMEK

	glPushMatrix(); // POCISK
	if (lot_kuli < zasieg_pocisku)
	{
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(x, -z, 0.0f);
		glRotatef(-angle * 57.2957795, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 0.0f + lot_kuli, 20.0f);

		gluSphere(Snieg, 5, 50, 50);

		glGetFloatv(GL_MODELVIEW_MATRIX, pocisk_position);
		//std::cout << "Pozycja kuli: " << pocisk_position[12] << " " << pocisk_position[13] << " " << pocisk_position[14] << "\n";

		//colision
		dx = pocisk_position[12] - ludzik_position[12];
		dy = pocisk_position[13] - (ludzik_position[13] + 10.0f);
		dz = pocisk_position[14] - ludzik_position[14];
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 10.0f)
		{
			//std::cout << "KOLIZJA!!!" << "\n";
			ludzik_dead = true;
		}
		lot_kuli += 5.0f;
	}
	glPopMatrix(); // !POCISK

    glPopMatrix(); // !macierz_glowna
	glFlush(); // KONIEC RYSOWANIA
}

void glScene::KeyboardFunction(char _key, int, int)
{
	if(_key == 32)
	{
		rot_[0] = rot_[1] = rot_[2] = 0.0;
		Resize(winx_, winy_);
	}
	repaintRequest_ = GL_TRUE;
}


void glScene::KeyboardFunction(int _key, int, int)
{
    GLfloat fraction = 5.0f;

    if(_key == GLUT_KEY_RIGHT)
    {
        angle += 0.08f;
        lx = sin(angle);
        lz = -cos(angle);
    }
	else if(_key == GLUT_KEY_LEFT)
	{
        angle -= 0.08f;
        lx = sin(angle);
        lz = -cos(angle);
	}
	else if(_key == GLUT_KEY_UP)
    {
        x += lx * fraction;
        z += lz * fraction;
    }
	else if(_key == GLUT_KEY_DOWN)
    {
        x -= lx * fraction;
        z -= lz * fraction;
    }
	else if(_key == GLUT_KEY_PAGE_UP)
    {
        rot_[2] += 1.0f;
    }
	else if(_key == GLUT_KEY_PAGE_DOWN)
    {
        rot_[2] -= 1.0f;
    }
    else if(_key == '+' )
    {
        glTranslatef(0.0f,0,50.0f);
    }
    else if(_key == '-')
    {
        glTranslatef(0.0f,0,-50.0f);
    }
	else if (_key == ' ')
	{
		lot_kuli = 1.0f;
	}
    repaintRequest_ = GL_TRUE;
}

void glScene::MouseMove(int, int)
{
}
void glScene::MouseLBDown(int, int)
{
}
void glScene::MouseLBUp(int, int)
{
}

void glScene::Timer()
{
	repaintRequest_ = GL_TRUE;
    //rot_[2] += 0.5f;
    //if (rot_[3] > 360) rot_[3] -= 360.0f;
}

bool glScene::WantTimer()
{
	return true;
}

int glScene::GetTimer()
{
	return 10;
}

void drawObject(OBJ obj, float scale, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < obj.vertexIndices.size(); i++)
	{
		glNormal3f(obj.normalCoordinates[obj.normalIndices[i].f1].x, obj.normalCoordinates[obj.normalIndices[i].f1].y, obj.normalCoordinates[obj.normalIndices[i].f1].z);
		glTexCoord2f(obj.textureCoordinates[obj.textureIndices[i].f1].x, obj.textureCoordinates[obj.textureIndices[i].f1].y);
		glVertex3f(obj.vertexCoordinates[obj.vertexIndices[i].f1].x*scale, obj.vertexCoordinates[obj.vertexIndices[i].f1].y*scale, obj.vertexCoordinates[obj.vertexIndices[i].f1].z*scale);
		glNormal3f(obj.normalCoordinates[obj.normalIndices[i].f2].x, obj.normalCoordinates[obj.normalIndices[i].f2].y, obj.normalCoordinates[obj.normalIndices[i].f3].z);
		glTexCoord2f(obj.textureCoordinates[obj.textureIndices[i].f2].x, obj.textureCoordinates[obj.textureIndices[i].f2].y);
		glVertex3f(obj.vertexCoordinates[obj.vertexIndices[i].f2].x*scale, obj.vertexCoordinates[obj.vertexIndices[i].f2].y*scale, obj.vertexCoordinates[obj.vertexIndices[i].f2].z*scale);
		glNormal3f(obj.normalCoordinates[obj.normalIndices[i].f3].x, obj.normalCoordinates[obj.normalIndices[i].f3].y, obj.normalCoordinates[obj.normalIndices[i].f3].z);
		glTexCoord2f(obj.textureCoordinates[obj.textureIndices[i].f3].x, obj.textureCoordinates[obj.textureIndices[i].f3].y);
		glVertex3f(obj.vertexCoordinates[obj.vertexIndices[i].f3].x*scale, obj.vertexCoordinates[obj.vertexIndices[i].f3].y*scale, obj.vertexCoordinates[obj.vertexIndices[i].f3].z*scale);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


