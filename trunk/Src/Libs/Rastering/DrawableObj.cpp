#include "StdAfx.h"

#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <list>
#include <vector>
#include <exception>
#include <fstream>

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "DrawableObj.h"

class Point3D
{
public:
	Point3D(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f %f", &X, &Y, &Z) != 3)
			throw std::exception("Couldn't read line into Point3D");
	}

	Point3D() {}

	void Normalize()
	{
		float length = sqrt(X*X + Y*Y + Z*Z);
		
		if (length == 0)
			return; //can't devide by 0

		X /= length;
		Y /= length;
		Z /= length;
	}

	float X, Y, Z;
};

class Point2D
{
public:
	Point2D(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f", &X, &Y) != 2)
			throw std::exception("Couldn't read line into Point2D");
	}

	Point2D() {}

	float X, Y;
};

class Face
{
public:
	Face(const std::string &arguemnts)
	{
		if (sscanf(arguemnts.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d"
			, vertices, texture, normals, vertices + 1, texture + 1, normals + 1, vertices + 2, texture + 2, normals + 2) != 9)
			throw std::exception("Can't parse argument in constructor for face");
		for (int i = 0; i < 3; i++)
		{
			vertices[i]--;
			texture[i]--;
			normals[i]--;
		}
	}

	Face(){};

	unsigned vertices[3], texture[3], normals[3];
};

DrawableObj::DrawableObj(const std::string &fileName, const std::string &objName, float scale) : _listNum(-1) // so that we wont accedently delete a list
{
	std::ifstream file(fileName.c_str());

	bool readingObject = false;
	std::string currentMtllib;
	std::string mtlObjName;
	
	std::vector<Point3D> vertices(0);
	std::vector<Point2D> texturePoints(0);
	std::vector<Point3D> normals(0);
	std::list<Face> faces;

	while (!file.eof())
	{
		char buffer[1024];
		file.getline(buffer, 1024);
		std::string line(buffer);
		
		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if (command == "mtllib")
			currentMtllib = arguments;
		else if ((command == "g") && (arguments == objName))
			readingObject = true;
		
		if (readingObject)
		{
			if (command == "v")
				vertices.push_back(Point3D(arguments));
			else if (command == "vt")
				texturePoints.push_back(Point2D(arguments));
			else if (command == "vn")
				normals.push_back(Point3D(arguments));
			else if (command == "usemtl")
				mtlObjName = arguments;
			else if (command == "f")
				faces.push_back(Face(arguments));
		}
	}

	if (!readingObject)
		throw std::exception("Couldn't find object in file when trying to construct DrawableObj");

	if (0 == (_listNum = glGenLists(1)))
		throw std::exception("Can't generate display list while creating a drawable object.");

	glNewList(_listNum, GL_COMPILE);
		glBegin(GL_TRIANGLES);

		for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		{
			for (int i = 0; i < 3; i++)
			{
				normals[(*iter).normals[i]].Normalize();
				glColor3f(0.4f, 0.7f, 0.6f);
				glNormal3f(normals[(*iter).normals[i]].X, normals[(*iter).normals[i]].Y, normals[(*iter).normals[i]].Z);
				glVertex3f(vertices[(*iter).vertices[i]].X * scale, vertices[(*iter).vertices[i]].Y * scale, vertices[(*iter).vertices[i]].Z * scale);
			}
		}

		glEnd();
	glEndList();


}

void DrawableObj::Draw()
{
	glCallList(_listNum);
}

DrawableObj::~DrawableObj()
{
	if (_listNum > 0)
		glDeleteLists(_listNum, 1);
}
