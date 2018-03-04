#include <windows.h>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Vertex.h"
#include "Coordinate.h"
#include <utility>

#ifndef OBJ_H
#define OBJ_H

class OBJ
{
public:
	std::string path;
	/*std::vector<Vertex> vertexCoordinates;
	std::vector<Coordinate> vertexIndices;
	std::vector<Vertex> textureCoordinates;
	std::vector<Coordinate> textureIndices;
	std::vector<Vertex> normalCoordinates;
	std::vector<Coordinate> normalIndices;*/

	Vertex * vertexCoordinates;
	Coordinate * vertexIndices;
	Vertex * textureCoordinates;
	Coordinate * textureIndices;
	Vertex * normalCoordinates;
	Coordinate * normalIndices;

public:
	OBJ(std::string path);

    ~OBJ(){}

	static void drawObject(OBJ obj, GLfloat scale, GLuint texture);
};

OBJ::OBJ(std::string path)
{
	this->path = path;
	std::ifstream image;
	std::string line;
	std::string temp;
	float v_temp;
	float v1;
	float v2;
	float v3;
	unsigned short f_temp;
	unsigned short fv1;
	unsigned short fv2;
	unsigned short fv3;

	unsigned short ft1;
	unsigned short ft2;
	unsigned short ft3;

	unsigned short fn1;
	unsigned short fn2;
	unsigned short fn3;

	image.open(this->path, std::ios::in);

	if (image.is_open())
	{
		//--------------WERTEXY------------//

		do
		{
			std::getline(image, line);
		} while (line[0] != 'v');

		std::stringstream ss;

		ss << line;
		ss >> temp;
		ss >> v1;
		ss >> v2;
		ss >> v3;
		Vertex * vertex = new Vertex(v1, v2, v3);
		vertexCoordinates.push_back(*vertex);
		vertexCoordinates.push_back(*vertex); // bo indexy sie zaczynaja od 1
		delete vertex;

		//std::cout << 1 << " : " << vertexCoordinates[1].x << " " << vertexCoordinates[1].y << " " << vertexCoordinates[1].z << "\n";

		int i = 2;
		while (true)
		{
			ss.clear();
			std::getline(image, line);
			if (line[0] != 'v' || line[1] != ' ') break;
			ss << line;
			ss >> temp;
			ss >> v1;
			ss >> v2;
			ss >> v3;
			Vertex * vertex = new Vertex(v1, v2, v3);
			vertexCoordinates.push_back(*vertex);
			delete vertex;

			//std::cout << i << " : " << vertexCoordinates[i].x << " " << vertexCoordinates[i].y << " " << vertexCoordinates[i].z << "\n";
			++i;
		}
		//std::cout << i << "\n";

		//---------------KOORDYNATY_TEKSTURY-----------------//

		//do
		//{
		//	std::getline(image, line);
		//} while (line[0] != 'v' && line[1] != 't');

		//std::cout << line << "\n";

		ss.clear();
		ss << line;
		ss >> temp;
		ss >> v1;
		ss >> v2;
		Vertex * vertex2 = new Vertex(v1, v2, 0.0f);
		textureCoordinates.push_back(*vertex2);
		textureCoordinates.push_back(*vertex2); // bo indexy sie zaczynaja od 1
		delete vertex2;

		//std::cout << 1 << " : " << textureCoordinates[1].x << " " << textureCoordinates[1].y << " " << textureCoordinates[1].z << "\n";
		i = 2;
		while (true)
		{

			ss.clear();
			std::getline(image, line);

			if (line[0] != 'v' || line[1] != 't') break;
			ss << line;
			ss >> temp;
			ss >> v1;
			ss >> v2;
			Vertex * vertex = new Vertex(v1, v2, 0.0f);

			textureCoordinates.push_back(*vertex);

			delete vertex;

			//std::cout << i << " : " << textureCoordinates[i].x << " " << textureCoordinates[i].y << " " << textureCoordinates[i].z << "\n";
			++i;


		}

		//----------------WEKTORY_NORMALNE----------------//

		//do
		//{
		//std::getline(image, line);
		//} while (line[0] != 'v' && line[1] != 'n');

		ss.clear();
		ss << line;
		ss >> temp;
		ss >> v1;
		ss >> v2;
		ss >> v3;
		Vertex * vertex3 = new Vertex(v1, v2, v3);
		normalCoordinates.push_back(*vertex3);
		normalCoordinates.push_back(*vertex3); // bo indexy sie zaczynaja od 1
		delete vertex3;

		//std::cout << 1 << " : " << normalCoordinates[1].x << " " << normalCoordinates[1].y << " " << normalCoordinates[1].z << "\n";

		i = 2;
		while (true)
		{
			ss.clear();
			std::getline(image, line);
			if (line[0] != 'v' || line[1] != 'n') break;
			ss << line;
			ss >> temp;
			ss >> v1;
			ss >> v2;
			ss >> v3;
			Vertex * vertex = new Vertex(v1, v2, v3);
			normalCoordinates.push_back(*vertex);
			delete vertex;

			//std::cout << i << " : " << normalCoordinates[i].x << " " << normalCoordinates[i].y << " " << normalCoordinates[i].z << "\n";
			++i;
		}
		//std::cout << i << "\n";

		//---------------INDEKSY_F--------------//

		do
		{
			std::getline(image, line);
		} while (line[0] != 'f');

		ss.clear();

		for (unsigned int j = 0; j < line.length(); ++j)
		{
			if (line[j] == '/') line[j] = ' ';
		}

		ss << line;
		//std::cout << line << "\n";
		ss >> temp;

		ss >> fv1;
		ss >> ft1;
		ss >> fn1;

		ss >> fv2;
		ss >> ft2;
		ss >> fn2;

		ss >> fv3;
		ss >> ft3;
		ss >> fn2;

		Coordinate * coord_fv = new Coordinate(fv1, fv2, fv3);
		Coordinate * coord_ft = new Coordinate(ft1, ft2, ft3);
		Coordinate * coord_fn = new Coordinate(fn1, fn2, fn3);
		vertexIndices.push_back(*coord_fv);
		textureIndices.push_back(*coord_ft);
		normalIndices.push_back(*coord_fn);
		delete coord_fv;
		delete coord_ft;
		delete coord_fn;

		//std::cout << 0 << " : " << vertexIndices[0].f1 << " " << vertexIndices[0].f2 << " " << vertexIndices[0].f3 << "\n";
		//std::cout << 0 << " : " << textureIndices[0].f1 << " " << textureIndices[0].f2 << " " << textureIndices[0].f3 << "\n";
		//std::cout << 0 << " : " << normalIndices[0].f1 << " " << normalIndices[0].f2 << " " << normalIndices[0].f3 << "\n";

		i = 1;
		while (true)
		{
			ss.clear();
			std::getline(image, line);
			if (line[0] != 'f') break;

			for (unsigned int j = 0; j < line.length(); ++j)
			{
				if (line[j] == '/') line[j] = ' ';
			}

			ss << line;
			ss >> temp;

			ss >> fv1;
			ss >> ft1;
			ss >> fn1;

			ss >> fv2;
			ss >> ft2;
			ss >> fn2;

			ss >> fv3;
			ss >> ft3;
			ss >> fn2;

			Coordinate * coord_fv = new Coordinate(fv1, fv2, fv3);
			Coordinate * coord_ft = new Coordinate(ft1, ft2, ft3);
			Coordinate * coord_fn = new Coordinate(fn1, fn2, fn3);
			vertexIndices.push_back(*coord_fv);
			textureIndices.push_back(*coord_ft);
			normalIndices.push_back(*coord_fn);
			delete coord_fv;
			delete coord_ft;
			delete coord_fn;
			//std::cout << i << " : " << vertexIndices[i].f1 << " " << vertexIndices[i].f2 << " " << vertexIndices[i].f3 << "\n";
			//std::cout << i << " : " << normalIndices[i].f1 << " " << normalIndices[i].f2 << " " << normalIndices[i].f3 << "\n";
			//std::cout << i << " : " << textureIndices[i].f1 << " " << textureIndices[i].f2 << " " << textureIndices[i].f3 << "\n";
			++i;
		}
		//std::cout << i << "\n";
	}
}

void OBJ::drawObject(OBJ obj, GLfloat scale, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < obj.vertexIndices.size(); i++)
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

#endif // !OBJ_H
