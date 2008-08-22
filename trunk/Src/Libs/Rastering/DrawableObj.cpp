#include "StdAfx.h"

#include <stdlib.h>

#include <iostream>
#include <list>
#include <vector>
#include <exception>
#include <fstream>

#include "DrawableObj.h"

class 3DPoint
{
public:
	3DPoint(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f %f", &X, &Y, &Z) != 3)
			throw std::exception("Couldn't read line into 3DPoint");
	}

	void Normalize()
	{
		float length = sqrt(X*X + Y*Y + Z*Z);
		
		if (length == 0)
			return; //can't devide by 0

		X \= length;
		Y \= length;
		Z \= length;
	}

	float X, Y, Z;
};

class 2DPoint
{
	2DPoint(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f", &X, &Y) != 2)
			throw std::exception("Couldn't read line into 2DPoint");
	}

	float X, Y;
};

class Face
{
public:
	Face(const std::string &arguemnts)
	{
		if (sscanf(arguemnts.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d"
			, vertices, texture, normals, vertices + 1, texture + 1, normals + 1, vertices + 2, texture + 2, normals + 2) != 9)
			throw std::exception("Can't parse arguement in constructor for face");
	}

	unsigned vertices[3], texture[3], normals[3];
};

DrawableObj::DrawableObj(const std::string &fileName, const std::string &objName) : _listNum(-1) // so that we wont accedently delete a list
{
	std::ifstream(fileName) file;

	bool readingObject = false;
	std::string currentMtllib;
	std::string mtlObjName;
	
	std::vector<3DPoint> vertices(0);
	std::vector<2DPoint> texturePoints(0);
	std::vector<3DPoint> normals(0);
	std::list<Face> faces;

	while (!file.eof())
	{
		std::string line(file.getline());
		int commandEnd(line.find(' ', 0))
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if (command == "mtllib")
			currentMtllib = arguments;
		elseif ((command == "g") && (arguments == objName))
			readingObject = true;
		
		if (readingObject)
		{
			if ((command == "v") && (readingObject))
				vertices.push_back(3DPoint(arguments));
			else if ((command == "vt") && (readingObject))
				texturePoints.push_back(2DPoint(arguments));
			else if ((command == "vn") && (readingObject))
				normals.push_back(3DPoint(arguments));
			else if ((command == "usemtl") && readingObject)
				mtlObjName = arguments;
			else if ((command == "f") && readingObject)
				faces->push_back(Face(arguments));
		}
	}

	if (!readingObject)
		throw std::exception("Couldn't find object in file when trying to construct DrawableObj");

	if (0 == (_listNum = glGenLists(1))
		throw std::exception("Can't generate display list while creating a drawable object.");

	glNewList(_listNum, GL_COMPILE);
		glBegin(GL_TRIANGLES);

		for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		{
			for (int i = 0; i < 3; i++)
			{
				normals[(*iter).normals[i]].Normalize();
				glNormal3f(normals[(*iter).normals[i]].X, normals[(*iter).normals[i]].Y, normals[(*iter).normals[i]].Z);
				glVertex3f(vertices[(*iter).vertices[i]].X, vertices[(*iter).vertices[i]].Y, vertices[(*iter).vertices[i]].Z);
			}
		}

		glEnd();
	glEndList();
}

void DrawableObj::Draw()
{
	glCallList(_listNum);
}

DrawableObj::~DrawableObj(void)
{
	if (_listNum > 0)
		glDeleteLists(_listNum, 1);
}
