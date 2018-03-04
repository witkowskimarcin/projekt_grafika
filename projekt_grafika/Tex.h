
#include <cstdlib>
#include <iostream>

#include <gl/gl.h>/*naglowek dla opengl32*/
#include <gl/glu.h>/*naglowek dla glu32*/
#include <gl/freeglut.h> /*naglowek dla freeglut'a*/
#include <gl/glext.h> //naglowek do gluBuild2DMipmaps
#include <stdio.h>
#include <math.h>


#include <string>
#include <fstream>
#include <sstream>


#define _USE_MATH_DEFINES
#include <cmath>

class Tex
{
public:

    unsigned char header[54];	//nag³ówek pliku BMP
    unsigned int dataPos;	//pozycja, gdzie znajduje siê plik
    unsigned int width, height;
    unsigned int imageSize;	//rozmiae pliku
    unsigned char * data;	//tabela z pikselami
    FILE * file;			//zmienna przechowuj¹ca pik
    void TexDec(GLuint name);
    GLuint IDtext[5];
    GLuint loadBMP (const char * imagepath);
    GLuint ON_tex(GLuint name);
};
    static int numerLT=1;
