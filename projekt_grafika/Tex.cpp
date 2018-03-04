#include "Tex.h"

GLuint Tex::loadBMP(const char * imagepath)
{
	file = fopen(imagepath, "rb");	//otwarcie pliku
	fread(header, 1, 54, file);		//odczytanie nag³ówka
	dataPos = *(int*)&(header[0x0A]);	//odczytanie pozycji
	imageSize = *(int*)&(header[0x22]);	//odczytanie rozmiaru
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	data = new unsigned char[imageSize];	//nowa tabela do przechowania pikseli tekstury
	fread(data, 1, imageSize, file);		//dodanie pikseli do tej tabeli
	fclose(file);	//zamkniêcie pliku

	GLuint texture = 0;
	return texture;
}
GLuint Tex::ON_tex(GLuint name)
{
	glBindTexture(GL_TEXTURE_2D, name);
	GLuint texture = 0;
	return texture;
}

void Tex::TexDec(GLuint name)
{

}
