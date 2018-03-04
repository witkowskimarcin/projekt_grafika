#include <windows.h> /*naglowek dla windowsa*/
#include <gl/gl.h>/*naglowek dla opengl32*/
#include <gl/glu.h>/*naglowek dla glu32*/
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
#include "vmath.h"
#include "SimpleFigures.h"

/*OKNO*/
GLint glScene::winx_ = 0;
GLint glScene::winy_ = 0;

GLfloat przemieszczenie = 0.0f; //ZMIENNA PRZEMIESZCZENIA
GLfloat kat = 0.0f; //ZMIENNA KAT

// obiekty potrzebne do załadowania tekstury
Tex texture;
GLuint textures[30];

// wektory światła
GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position[] = { -50.0f, -50.0f, 100.0f, 1.0f };
GLfloat light_position1[] = { -50.0f, -50.0f, -300.0f, 1.0f };
GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat high_shininess[] = { 10.0f };

// zmienne potrzebne do obracania kamerą (światem)
GLfloat angle = 0.0f;
GLfloat lx=0.0f,lz=-1.0f;
GLfloat x=0.0f,y=1.0f,z=5.0f;

// modele OBJ
OBJ ludzik("objects/ludzik.obj");
OBJ gun("objects/gun.obj");
OBJ domek("objects/dom.obj");
OBJ domek2("objects/dom2.obj");
OBJ drzewo("objects/tree.obj");
OBJ human1("objects/human1.obj");
//OBJ human2("objects/human2.obj");
//OBJ human3("objects/human3.obj");
OBJ pionek("objects/pionek.obj");

GLUquadricObj *Snieg = gluNewQuadric();

// flagi
bool fire = false;
bool one = true;
bool resetTimer = false;
bool ludzik_dead = false;
bool human1_dead = false;
bool human2_dead = false;
bool human3_dead = false;
bool pionek1_dead = false;
bool pionek2_dead = false;
bool pionek3_dead = false;
bool pionek4_dead = false;
bool pionek5_dead = false;
bool pionek6_dead = false;
bool pionek7_dead = false;
bool pionek8_dead = false;
bool pionek9_dead = false;
bool pionek10_dead = false;
bool flaga_kula = false;
bool door1 = false;
bool door2 = false;

// zmienne do kolizji
float time = 1;
float lot_kuli = 10000.0f;
float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;
float distance = 0.0f;
GLfloat camera_position[16];
GLfloat ludzik_position[16];
GLfloat human1_position[16];
GLfloat human2_position[16];
GLfloat human3_position[16];
GLfloat pocisk_position[16];
GLfloat pionek1_position[16];
GLfloat pionek2_position[16];
GLfloat pionek3_position[16];
GLfloat pionek4_position[16];
GLfloat pionek5_position[16];
GLfloat pionek6_position[16];
GLfloat pionek7_position[16];
GLfloat pionek8_position[16];
GLfloat pionek9_position[16];
GLfloat pionek10_position[16];
GLfloat zasieg_pocisku = 600.0f;
GLfloat door1_position[16];

// manipulacja światem +/-
GLfloat oddal = 0.0f;

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
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// światło
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    //glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    //glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // tekstury

	textures[1] = texture.loadBMP("textures/gun.bmp");					//załadowanie pliku (z folderu z projektem)
	glGenTextures(1, &textures[1]);										//generowanie tekstury
	glBindTexture(GL_TEXTURE_2D, textures[1]);							//ustawienie tej tekstury (nią będą kolorowane przedmioty)
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[2] = texture.loadBMP("textures/dom.bmp");
	glGenTextures(1, &textures[2]);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[3] = texture.loadBMP("textures/trawa.bmp");
	glGenTextures(1, &textures[3]);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[4] = texture.loadBMP("textures/build.bmp");
	glGenTextures(1, &textures[4]);	
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[5] = texture.loadBMP("textures/asfalt.bmp");
	glGenTextures(1, &textures[5]);	
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[6] = texture.loadBMP("textures/chodnik.bmp");
	glGenTextures(1, &textures[6]);
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[7] = texture.loadBMP("textures/beton.bmp");
	glGenTextures(1, &textures[7]);
	glBindTexture(GL_TEXTURE_2D, textures[7]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[8] = texture.loadBMP("textures/build2.bmp");
	glGenTextures(1, &textures[8]);
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[9] = texture.loadBMP("textures/tree.bmp");
	glGenTextures(1, &textures[9]);	
	glBindTexture(GL_TEXTURE_2D, textures[9]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[10] = texture.loadBMP("textures/human1.bmp");
	glGenTextures(1, &textures[10]);
	glBindTexture(GL_TEXTURE_2D, textures[10]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[11] = texture.loadBMP("textures/human2.bmp");
	glGenTextures(1, &textures[11]);						
	glBindTexture(GL_TEXTURE_2D, textures[11]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[12] = texture.loadBMP("textures/human3.bmp");
	glGenTextures(1, &textures[12]);
	glBindTexture(GL_TEXTURE_2D, textures[12]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[13] = texture.loadBMP("textures/wood.bmp");
	glGenTextures(1, &textures[13]);
	glBindTexture(GL_TEXTURE_2D, textures[13]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[14] = texture.loadBMP("textures/pionek.bmp");
	glGenTextures(1, &textures[14]);
	glBindTexture(GL_TEXTURE_2D, textures[14]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[15] = texture.loadBMP("textures/tapeta_kregielnia.bmp");
	glGenTextures(1, &textures[15]);
	glBindTexture(GL_TEXTURE_2D, textures[15]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[16] = texture.loadBMP("textures/czarny.bmp");
	glGenTextures(1, &textures[16]);
	glBindTexture(GL_TEXTURE_2D, textures[16]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[17] = texture.loadBMP("textures/door.bmp");
	glGenTextures(1, &textures[17]);
	glBindTexture(GL_TEXTURE_2D, textures[17]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

	textures[18] = texture.loadBMP("textures/dom2.bmp");
	glGenTextures(1, &textures[18]);
	glBindTexture(GL_TEXTURE_2D, textures[18]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width, texture.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	delete[] texture.data;

    glLoadIdentity();
}

void glScene::CleanRC()
{

}

void glScene::RenderScene()
{
	//glLoadIdentity();
	//GLUquadricObj *niebo = gluNewQuadric();

	gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	//std::cout << "Twoja pozycja:" << x << " : " << y << " : " << z << "\n";

	//malujemy scene, mozna skasowac flage aby scena ciagle sie nie odmalowywala
	repaintRequest_ = GL_FALSE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszcenie okna

	glPushMatrix(); // glowna macierz
	glTranslatef(0.0f, -30.0f, 0.0f); //cofniecie perspektywy do tyłu
	glRotatef(-90 + rot_[2], 1.0f, 0.0f, 0.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, camera_position);
	//std::cout << "Twoja pozycja:" << camera_position[12] << " : " << camera_position[13] << " : " << camera_position[14] << "\n";

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix(); // TRAWA
	{
		glTranslatef(200.0f, -100.0f, -1.0f);
		glScalef(10.0f, 10.0f, 1.0f);
		SimpleFigures::drawQuad(100.0f, textures[3], 0.0f,0.0f,0.0f,100.0f);
	}
	glPopMatrix(); // !TRAWA

	glPushMatrix(); // ULICA
	{
		glTranslatef(0.0f, -0.05f, -0.02f);
		glScalef(0.8f, 5.0f, 1.0f);
		SimpleFigures::drawQuad(100.0f, textures[5],0.0f,0.0f,0.0f,100.0f);
	}
	glPopMatrix(); // !ULICA

	glPushMatrix(); // CHODNIK
	{
		glTranslatef(100.0f, 10.0f, -0.05f);

		glPushMatrix();
		{
			glTranslatef(100.0f, 0.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(100.0f, 0.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(100.0f, 0.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
		}
		glPopMatrix();

		glPushMatrix();
		{
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
		}
		glPopMatrix();

		glTranslatef(-200.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, 100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
			glTranslatef(0.0f, -100.0f, 0.0f);
			SimpleFigures::drawQuad(50.0f, textures[6]);
		}
		glPopMatrix();

	}
	glPopMatrix(); // !CHODNIK

	glPushMatrix(); // BETON
	{
		glTranslatef(-450.0f, 100.0f, -0.05f);
		glScalef(1.0f, 2.0f, 1.0f);
		SimpleFigures::drawQuad(300.0f, textures[7]);
	}
	glPopMatrix(); // !BETON

	glPushMatrix(); // budynek_1
	{
		glTranslatef(-500.0f, 300.0f, 50.0f);
		glScalef(2.0f, 4.0f, 4.0f);
		glTranslatef(0.0f, 0.0f, 38.0f);
		SimpleFigures::drawCube(1.0f, textures[4]);
	}
	glPopMatrix(); // !budynek_1

	glPushMatrix(); // budynek_2
	{
		glTranslatef(-500.0f, -300.0f, 50.0f);
		glScalef(2.0f, 4.0f, 4.0f);
		glTranslatef(0.0f, 0.0f, 38.0f);
		SimpleFigures::drawCube(1.0f, textures[8]);
	}
	glPopMatrix(); // !budynek_2

	glPushMatrix(); // DOMEK
	{
		glTranslatef(400.0f, -155.0f, 30.0f);
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		//OBJ::drawObject(domek, 20.0f, textures[2]);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 100.0f, 0.0f);
		OBJ::drawObject(domek2, 3.0f, textures[18]);
	}
	glPopMatrix(); // !DOMEK

	glPushMatrix(); // DRZEWA_1
	{
		glTranslatef(200.0f, -600.0f, 0.0f);
		OBJ::drawObject(drzewo, 100.0f, textures[9]);
		glTranslatef(0.0f, 200.0f, 0.0f);
		OBJ::drawObject(drzewo, 100.0f, textures[9]);
		glTranslatef(0.0f, 200.0f, 0.0f);
		OBJ::drawObject(drzewo, 100.0f, textures[9]);

		glPushMatrix();
		{
			glTranslatef(200.0f, 0.0f, 0.0f);
			OBJ::drawObject(drzewo, 100.0f, textures[9]);

			glPushMatrix();
			{
				glTranslatef(0.0f, -200.0f, 0.0f);
				OBJ::drawObject(drzewo, 100.0f, textures[9]);
				glTranslatef(0.0f, -200.0f, 0.0f);
				OBJ::drawObject(drzewo, 100.0f, textures[9]);
			}
			glPopMatrix();

			glTranslatef(200.0f, 0.0f, 0.0f);
			OBJ::drawObject(drzewo, 100.0f, textures[9]);
			glTranslatef(0.0f, -200.0f, 0.0f);
			OBJ::drawObject(drzewo, 100.0f, textures[9]);
			glTranslatef(0.0f, -200.0f, 0.0f);
			OBJ::drawObject(drzewo, 100.0f, textures[9]);
		}
		glPopMatrix();
	}
	glPopMatrix(); // ! DRZEWA_1

	glPushMatrix(); // BUDYNEK_3_KREGIELNIA
	{
		glPushMatrix();
		{
			glTranslatef(200.0f, 517.5f, 30.0f);
			glScalef(0.1f, 1.65f, 0.6f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(200.0f, 350.0f, 30.0f);
			glScalef(0.1f, 1.0f, 0.6f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(200.0f, 450.0f, 100.0f);
			glScalef(0.1f, 3.0f, 0.8f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(690.0f, 450.0f, 70.0f);
			glScalef(0.1f, 3.0f, 1.4f);
			SimpleFigures::drawCube(1.0f, textures[15] , textures[7], textures[7], textures[7], textures[7], textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(450.0f, 450.0f, 150.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.1f, 3.0f, 5.0f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(450.0f, 450.0f, 150.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.1f, 3.0f, 5.0f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(450.0f, 590.0f, 65.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.1f, 1.5f, 4.8f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(450.0f, 300.0f, 65.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.1f, 1.5f, 4.8f);
			SimpleFigures::drawCube(1.0f, textures[7]);
		}
		glPopMatrix();
		glPushMatrix(); // PODLOGA_KREGIELNI
		{
			glTranslatef(450.0f, 450.0f, 1.0f);
			glScalef(2.5f, 1.5f, 1.0f);
			SimpleFigures::drawQuad(100.0f, textures[13]);
		}
		glPopMatrix(); // !PODLOGA_KREGIELNI
		glPushMatrix(); // PODLOGA_KREGIELNI_2
		{
			glTranslatef(450.0f, 450.0f, 2.0f);
			glScalef(2.4f, 0.2f, 1.0f);
			SimpleFigures::drawQuad(100.0f, textures[16]);
		}
		glPopMatrix(); // !PODLOGA_KREGIELNI_2
		glPushMatrix(); // PRZED_KREGIELNI
		{
			glTranslatef(180.0f, 420.0f, -0.5f);
			//glScalef(2.4f, 0.2f, 1.0f);
			SimpleFigures::drawQuad(30.0f, textures[6]);
		}
		glPopMatrix(); // !PRZED_KREGIELNI
		glPushMatrix(); // DZWI_KREGIELNI
		{
			glTranslatef(202.5f, 417.5f, 30.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			
			if (door1)
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, -20.0f, -17.5f);
			}

			if (!door1) glGetFloatv(GL_MODELVIEW_MATRIX, door1_position);
			glScalef(1.0f, 0.6f, 1.2f);
			SimpleFigures::drawQuad(30.0f, textures[17]);
		}
		glPopMatrix(); // !DZWI_KREGIELNI
		glTranslatef(0.0f, 0.0f, 32.0f);
		glPushMatrix(); // KREGLE_1
		{
			glTranslatef(650.0f, 480.0f, 0.0f);
			if (pionek1_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek1_position);
			if(pionek1_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(-10.0f, 15.0f, 0.0f);
			if (pionek2_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek2_position);
			if (pionek2_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(-10.0f, 15.0f, 0.0f);
			if (pionek3_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek3_position);
			if (pionek3_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(10.0f, 15.0f, 0.0f);
			if (pionek4_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek4_position);
			if (pionek4_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(10.0f, 15.0f, 0.0f);
			if (pionek5_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek5_position);
			if (pionek5_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}
		}
		glPopMatrix(); // !KREGLE_1
		glPushMatrix(); // KREGLE_2
		{
			glTranslatef(0.0f, -150.0f, 0.0f);
			glTranslatef(650.0f, 500.0f, 0.0f);
			if (pionek6_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek6_position);
			if (pionek6_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(-10.0f, 15.0f, 0.0f);
			if (pionek7_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek7_position);
			if (pionek7_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(-10.0f, 15.0f, 0.0f);
			if (pionek8_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek8_position);
			if (pionek8_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(10.0f, 15.0f, 0.0f);
			if (pionek9_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek9_position);
			if (pionek9_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}

			glTranslatef(10.0f, 15.0f, 0.0f);
			if (pionek10_dead)
			{
				glTranslatef(30.0f, 0.0f, -25.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			}
			OBJ::drawObject(pionek, 20.0f, textures[14]);
			glGetFloatv(GL_MODELVIEW_MATRIX, pionek10_position);
			if (pionek10_dead)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(-30.0f, 0.0f, 25.0f);
			}
		}
		glPopMatrix(); // !KREGLE_2
	}
	glPopMatrix(); // !BUDYNEK_3_KREGIELNIA

	glPushMatrix(); // HUMAN_1
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-100.0f, 400.0f, -8.0f);
		//glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		if (human1_dead) glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		else glTranslatef(0.0f, 0.0f, 30.0f);
		OBJ::drawObject(human1, 72.0f, textures[10]);
		glTranslatef(0.0f, -15.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, human1_position);
		//std::cout << "Pozycja human1: " << human1_position[12] << " " << human1_position[13] << " " << human1_position[14] << "\n";
	}
	glPopMatrix(); // !HUMAN_1

	glPushMatrix(); // HUMAN_2
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-50.0f, 400.0f, -8.0f);
		if (human2_dead) glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		else glTranslatef(0.0f, 0.0f, 30.0f);
		OBJ::drawObject(human1, 72.0f, textures[11]);
		glTranslatef(0.0f, -15.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, human2_position);
	}
	glPopMatrix(); // !HUMAN_2

	glPushMatrix(); // HUMAN_3
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 400.0f, -8.0f);
		if (human3_dead) glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		else glTranslatef(0.0f, 0.0f, 30.0f);
		OBJ::drawObject(human1, 72.0f, textures[12]);
		glTranslatef(0.0f, -15.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, human3_position);
	}
	glPopMatrix(); // !HUMAN_3

	glPushMatrix(); // POCISK
	glColor3f(0.1f, 0.1f, 0.1f);
	glTranslatef(x, -z, 0.0f);
	//if (lot_kuli <= 10.0f)
	{
		glRotatef(-angle * 57.2957795f, 0.0f, 0.0f, 1.0f);
	}
	//else
	{
		//glRotatef(angle* 30.0f, 0.0f, 0.0f, 1.0f);
	}
	if (lot_kuli < zasieg_pocisku)
	{
		if(flaga_kula) glTranslatef(0.0f, 0.0f, -15.0f);
		glTranslatef(0.0f, 0.0f + lot_kuli, 20.0f);
		//std::cout << "lx: " << lx* 57.2957795f << " lz: " << lz* 57.2957795f << "\n ";
		//std::cout << "angle: " << -angle * 57.2957795f <<"\n ";


		//glTranslatef(lz* 57.2957795f, lx* 57.2957795f,0.0f );
		

		glGetFloatv(GL_MODELVIEW_MATRIX, pocisk_position);
		gluSphere(Snieg, 5, 50, 50);
		//std::cout << "Pozycja kuli: " << pocisk_position[12] << " " << pocisk_position[13] << " " << pocisk_position[14] << "\n";

		//colision_human1
		//std::cout << "Pozycja human1: " << human1_position[12] << " " << human1_position[13] << " " << human1_position[14] << "\n";
		dx = pocisk_position[12] - (human1_position[12]);
		dy = pocisk_position[13] - (human1_position[13]);
		dz = pocisk_position[14] - (human1_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 10.0f) 
		{
			lot_kuli = zasieg_pocisku;
			human1_dead = true;
		}

		//colision_human2
		dx = pocisk_position[12] - (human2_position[12]);
		dy = pocisk_position[13] - (human2_position[13]);
		dz = pocisk_position[14] - (human2_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 10.0f)
		{
			human2_dead = true;
			lot_kuli = zasieg_pocisku;
		}

		//colision_human3
		dx = pocisk_position[12] - (human3_position[12]);
		dy = pocisk_position[13] - (human3_position[13]);
		dz = pocisk_position[14] - (human3_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 10.0f)
		{
			human3_dead = true;
			lot_kuli = zasieg_pocisku;
		}

		//colision_pionek1
		//std::cout << "Pozycja pionek1: " << pionek1_position[12] << " " << pionek1_position[13] << " " << pionek1_position[14] << "\n";
		dx = pocisk_position[12] - (pionek1_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek1_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek1_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek1_dead = true;

		//colision_pionek2
		dx = pocisk_position[12] - (pionek2_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek2_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek2_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek2_dead = true;

		//colision_pionek3
		dx = pocisk_position[12] - (pionek3_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek3_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek3_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek3_dead = true;

		//colision_pionek4
		dx = pocisk_position[12] - (pionek4_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek4_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek4_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek4_dead = true;

		//colision_pionek5
		dx = pocisk_position[12] - (pionek5_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek5_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek5_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek5_dead = true;

		//colision_pionek6
		dx = pocisk_position[12] - (pionek6_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek6_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek6_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek6_dead = true;

		//colision_pionek7
		dx = pocisk_position[12] - (pionek7_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek7_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek7_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek7_dead = true;

		//colision_pionek8
		dx = pocisk_position[12] - (pionek8_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek8_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek8_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek8_dead = true;

		//colision_pionek9
		dx = pocisk_position[12] - (pionek9_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek9_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek9_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek9_dead = true;

		//colision_pionek10
		dx = pocisk_position[12] - (pionek10_position[12] - 4.0f);
		dy = pocisk_position[13] - (pionek10_position[13] - 25.0f);
		dz = pocisk_position[14] - (pionek10_position[14]);
		distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= 15.0f) pionek10_dead = true;

		lot_kuli += 5.0f;
	}
	glPopMatrix(); // !POCISK

    glPopMatrix(); // !macierz_glowna

	glPushMatrix();	// BRON
	{
		glTranslatef(0.0f, -30.0f, 0.0f + oddal); //cofniecie perspektywy do tyłu
		glRotatef(-90 + rot_[2], 1.0f, 0.0f, 0.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(x, -z, 0.0f);
		glRotatef(100.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-angle * 57.2957795f, 0.0f, 0.0f, 1.0f); // Rotate to correspond to the camera
		glTranslatef(28.0f, 15.0f, 20.0f);
		OBJ::drawObject(gun, 100.0f, textures[1]);
		glTranslatef(-50.0f, -15.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		OBJ::drawObject(human1, 72.0f, textures[11]);
	}
	glPopMatrix(); // !BRON

	glFlush(); // KONIEC RYSOWANIA
	glLoadIdentity();
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
        angle += 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
    }
	else if(_key == GLUT_KEY_LEFT)
	{
        angle -= 0.05f;
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
        rot_[2] += 10.0f;
    }
	else if(_key == GLUT_KEY_PAGE_DOWN)
    {
        rot_[2] -= 10.0f;
    }
    else if(_key == '+' )
    {
        //glTranslatef(0.0f,0,50.0f);
		oddal += 50.0f;
    }
    else if(_key == '-')
    {
       // glTranslatef(0.0f,0,-50.0f);
		oddal -= 50.0f;
    }
	else if (_key == 'z')
	{
		flaga_kula = false;
		lot_kuli = 0.0f;
	}
	else if (_key == ' ')
	{
		flaga_kula = true;
		lot_kuli = 0.0f;
	}
	else if (_key == 'r')
	{
		ludzik_dead = false;
		human1_dead = false;
		human2_dead = false;
		human3_dead = false;

		pionek1_dead = false;
		pionek2_dead = false;
		pionek3_dead = false;
		pionek4_dead = false;
		pionek5_dead = false;
		pionek6_dead = false;
		pionek7_dead = false;
		pionek8_dead = false;
		pionek9_dead = false;
		pionek10_dead = false;

		door1 = false;
		door2 = false;

	}
	else if (_key == 'p')
	{
		y += 10.0f;
	}
	else if (_key == 'k')
	{
		std::cout << "Pozycja kamera: " << camera_position[12] << " " << camera_position[13] << " " << camera_position[14] << "\n";
		std::cout << "Pozycja door1: " << door1_position[12] << " " << door1_position[13] << " " << door1_position[14] << "\n";

		//colision_doo1
		dx = door1_position[12];
		//dy = door1_position[13] - (camera_position[13]);
		dz = door1_position[14];
		distance = sqrt(dx*dx + dz*dz);
		if (distance <= 50.0f)
		{
			if (door1) door1 = false;
			else door1 = true;
		}

		if (door2) door2 = false;
		else door2 = true;
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

