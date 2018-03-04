#ifndef SIMPLEFIGURES_H
#define SIMPLEFIGURES_H

#include <gl/freeglut.h> /*naglowek dla freeglut'a*/
#include "OBJ.h"

class SimpleFigures
{
private:
	SimpleFigures() {}
public:
	static void drawQuad(GLfloat size, GLuint texture, GLfloat rotate_x = 0.0f, GLfloat rotate_y = 0.0f, GLfloat rotate_z = 0.0f, GLfloat ile_wektorow_normalnych=10.0f);
	static void drawSquare(GLfloat dl_boku_A, GLfloat skala_pojedynczego_kwadratu, GLuint texture);
	static void drawCube(GLfloat size, GLuint texture1);
	static void drawCube(GLfloat size, GLuint texture1, GLuint texture2, GLuint texture3, GLuint texture4, GLuint texture5, GLuint texture6);
};

void SimpleFigures::drawCube(GLfloat size, GLuint texture1)
{

	drawQuad(50.0f, texture1, 0.0f, 1.0f, 0.0f);
	glTranslatef(100.0f, 0.0f, 0.0f);
	drawQuad(50.0f, texture1, 0.0f, 1.0f, 0.0f);
	glTranslatef(-50.0f, 0.0f, -50.0f);
	drawQuad(50.0f, texture1);
	glTranslatef(0.0f, 0.0f, 100.0f);
	drawQuad(50.0f, texture1);
	glTranslatef(0.0f, -50.0f, -50.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	drawQuad(50.0f, texture1);
	glTranslatef(0.0f, 0.0f, 100.0f);
	drawQuad(50.0f, texture1);
}

void SimpleFigures::drawCube(GLfloat size, GLuint texture1, GLuint texture2, GLuint texture3, GLuint texture4, GLuint texture5, GLuint texture6)
{

	drawQuad(50.0f, texture1, 0.0f, 1.0f, 0.0f);
	glTranslatef(100.0f, 0.0f, 0.0f);
	drawQuad(50.0f, texture2, 0.0f, 1.0f, 0.0f);
	glTranslatef(-50.0f, 0.0f, -50.0f);
	drawQuad(50.0f, texture3);
	glTranslatef(0.0f, 0.0f, 100.0f);
	drawQuad(50.0f, texture4);
	glTranslatef(0.0f, -50.0f, -50.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	drawQuad(50.0f, texture5);
	glTranslatef(0.0f, 0.0f, 100.0f);
	drawQuad(50.0f, texture6);
}

void SimpleFigures::drawQuad(GLfloat size, GLuint texture, GLfloat rotate_x, GLfloat rotate_y, GLfloat rotate_z, GLfloat ile_wektorow_normalnych)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	if (rotate_x != 0.0f || rotate_y != 0.0f || rotate_z != 0.0f)
	{
		glRotatef(90.0f, rotate_x, rotate_y, rotate_z);

	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	{

		for (GLfloat i = -size; i < size; i += size/ ile_wektorow_normalnych)
		{
			for (GLfloat j = -size; j < size; j += size/ ile_wektorow_normalnych)
			{
				glNormal3f(0.0f + i, 0.0f + j, 1.0f);
				glNormal3f(0.0f + i, 0.0f + j, -1.0f);
			}
		}
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-size, -size, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(size, -size, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(size, size, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-size, size, 0.0f);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	if (rotate_x != 0.0f || rotate_y != 0.0f || rotate_z != 0.0f)
		glRotatef(-90.0f, rotate_x, rotate_y, rotate_z);
}

void SimpleFigures::drawSquare(GLfloat dl_boku_A, GLfloat skala_pojedynczego_kwadratu, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (GLfloat i = 0.0f; i < dl_boku_A; i += 1.0f)
	{
		for (GLfloat j = 0.0f; j < dl_boku_A; j += 1.0f)
		{
			glBegin(GL_TRIANGLE_FAN);
			{
				glNormal3f(0.0f, 0.0f, 1.0f);

				glTexCoord2f((i / dl_boku_A), (j / dl_boku_A));
				glVertex3f(-1.0f*skala_pojedynczego_kwadratu, -1.0f*skala_pojedynczego_kwadratu, 0.0f);

				glTexCoord2f((i / dl_boku_A), ((1.0f + j) / dl_boku_A));
				glVertex3f(1.0f*skala_pojedynczego_kwadratu, -1.0f*skala_pojedynczego_kwadratu, 0.0f);

				glTexCoord2f(((1.0f + i) / dl_boku_A), ((1.0f + j) / dl_boku_A));
				glVertex3f(1.0f*skala_pojedynczego_kwadratu, 1.0f*skala_pojedynczego_kwadratu, 0.0f);

				glTexCoord2f(((1.0f + i) / dl_boku_A), (j / dl_boku_A));
				glVertex3f(-1.0f*skala_pojedynczego_kwadratu, 1.0f*skala_pojedynczego_kwadratu, 0.0f);

			}
			glEnd();

			glTranslatef(0.0f, skala_pojedynczego_kwadratu, 0.0f);
		}
		glTranslatef(0.0f, -(skala_pojedynczego_kwadratu*dl_boku_A), 0.0f);
		glTranslatef(skala_pojedynczego_kwadratu, 0.0f, 0.0f);
	}
	glDisable(GL_TEXTURE_2D);
}

#endif // !SIMPLEFIGURES_H