#include "StdAfx.h"

#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <list>
#include <vector>
#include <exception>
#include <fstream>

#include <boost/format.hpp>

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "DrawableObj.h"

class Point3D
{
public:
	Point3D(const std::string& arguments)
	{
		LoadFromArguments(arguments);
	}

	void LoadFromArguments(const std::string& arguments)
	{
		if (sscanf(arguments.c_str(), "%f %f %f", Points, Points + 1, Points + 2) != 3)
			throw std::exception("Couldn't read line into Point3D");
	}

	Point3D() {}

	void Normalize()
	{
		float length = sqrt(Points[0]*Points[0] + Points[1]*Points[1] + Points[2]*Points[2]);
		
		if (length == 0)
			return; //can't devide by 0

		Points[0] /= length;
		Points[1] /= length;
		Points[2] /= length;
	}

	float Points[3];
};

class Point2D
{
public:
	Point2D(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f", Points, Points + 1) != 2)
			throw std::exception("Couldn't read line into Point2D");
	}

	Point2D() {}

	float Points[2];
};

class Face
{
public:
	Face(const std::string &arguments, const std::string& mtlObjName) : MtlObjName(mtlObjName)
	{
		if (sscanf(arguments.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d"
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
	std::string MtlObjName;
};

class MtlObj
{
public:
	Point3D ka, kd, ks;
	int illum;
	int ns;
};

std::string readLine(std::ifstream& stream)
{
	char buffer[1024];
	stream.getline(buffer, 1024);
	return std::string(buffer);
}

float DSin(float degrees)
{
	return (float)sin(degrees * (M_PI / 180));
}

float DCos(float degrees)
{
	return (float)cos(degrees * (M_PI / 180));
}

DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, const std::string &objName, float scale, float rotateX, float rotateY, float rotateZ) : _listNum(-1) // so that we wont accedently delete a list
{
	std::ifstream objFile((boost::format("%1%\\%2%") % directory % fileName).str().c_str());

	bool readingObject = false;
	std::string currentMtllib;
	std::string mtlObjName;
	
	std::vector<Point3D> vertices(0);
	std::vector<Point2D> texturePoints(0);
	std::vector<Point3D> normals(0);
	std::list<Face> faces;

	while (!objFile.eof())
	{
		std::string line(readLine(objFile));
		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if (command == "mtllib")
			currentMtllib = arguments;
		else if (command == "g")
			readingObject = (arguments == objName);
		
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
				faces.push_back(Face(arguments, mtlObjName));
		}
	}

	objFile.close();

	if (faces.size() == 0)
		throw std::exception("Couldn't find object in file when trying to construct DrawableObj");

	std::ifstream mtlFile((boost::format("%1%\\%2%") % directory % currentMtllib).str().c_str());

	std::string currentMtlObjName;
	std::map<std::string, MtlObj> mtlObjects;
	MtlObj currentMtlObj;

	while (!mtlFile.eof())
	{
		std::string line(readLine(mtlFile));
		
		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if ((command == "newmtl") || (command.empty()))
		{		
			if (!currentMtlObjName.empty())
				mtlObjects.insert(std::pair<std::string, MtlObj>(currentMtlObjName, currentMtlObj));

			currentMtlObjName = arguments;
		}
		else if (command == "Ka")
			currentMtlObj.ka.LoadFromArguments(arguments);
		else if (command == "Kd")
			currentMtlObj.kd.LoadFromArguments(arguments);
		else if (command == "Ks")
			currentMtlObj.ks.LoadFromArguments(arguments);
		else if (command == "illum")
			sscanf(arguments.c_str(), "%d", &currentMtlObj.illum);
		else if (command == "Ns")
			sscanf(arguments.c_str(), "%d", &currentMtlObj.ns);
	}

	mtlFile.close();

	float sumOfMass[3], maxPoint[3], minPoint[3];

	for (int i = 0; i < 3; i++)
	{
		sumOfMass[i] = 0;
		maxPoint[i] = std::numeric_limits<float>::min();
		minPoint[i] = std::numeric_limits<float>::max();
	}

	for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int coordIndex = 0; coordIndex < 3; coordIndex++)
			{
				sumOfMass[coordIndex] += vertices[(*iter).vertices[i]].Points[coordIndex];
				maxPoint[coordIndex] = std::max(maxPoint[coordIndex], vertices[(*iter).vertices[i]].Points[coordIndex]);
				minPoint[coordIndex] = std::min(minPoint[coordIndex], vertices[(*iter).vertices[i]].Points[coordIndex]);
			}
		}
	}

	float centerOfMass[3], size = 0;
	for (int i = 0; i < 3; i++)
	{
		centerOfMass[i] = sumOfMass[i] / (faces.size() * 3);
		size = std::max(size, maxPoint[i] - minPoint[i]);
	}

	_yMin = std::numeric_limits<float>::infinity();
	for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		for (int i = 0; i < 3; i++)
			_yMin = std::min(_yMin, (
				  (vertices[(*iter).vertices[i]].Points[0] - centerOfMass[0]) * (scale / size) * (-DSin(rotateX) * DCos(rotateY) * DCos(rotateZ) - DSin(rotateY) * DSin(rotateZ))
				+ (vertices[(*iter).vertices[i]].Points[1] - centerOfMass[1]) * (scale / size) * (DCos(rotateX) * DCos(rotateZ))
				+ (vertices[(*iter).vertices[i]].Points[2] - centerOfMass[2]) * (scale / size) * (-DSin(rotateX) * DSin(rotateY) * DCos(rotateZ) + DCos(rotateY) * DSin(rotateZ))));

	if (0 == (_listNum = glGenLists(1)))
		throw std::exception("Can't generate display list while creating a drawable object.");

	glNewList(_listNum, GL_COMPILE);
		
		glMatrixMode(GL_MODELVIEW);
		
		glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
		glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
		glScalef(scale / size, scale / size, scale / size);
		glTranslatef(-centerOfMass[0], -centerOfMass[1], -centerOfMass[2]);
		
		glBegin(GL_TRIANGLES);

		for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::map<std::string, MtlObj>::iterator mtlObjIter = mtlObjects.find((*iter).MtlObjName);

				if (mtlObjIter == mtlObjects.end())
					throw std::exception("Couldn't find mtlObj for a face I was trying to draw.");

				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (*mtlObjIter).second.ka.Points);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (*mtlObjIter).second.kd.Points);
				
				if ((*mtlObjIter).second.illum == 2)
				{
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (*mtlObjIter).second.ks.Points);
					glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat)(*mtlObjIter).second.ns);
				}
				else
				{
					float zeroArray[] = {0.0f, 0.0f, 0.0f};
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroArray);
				}
				
				normals[(*iter).normals[i]].Normalize();
				glNormal3fv(normals[(*iter).normals[i]].Points);
				glVertex3f(vertices[(*iter).vertices[i]].Points[0]
						 , vertices[(*iter).vertices[i]].Points[1]
						 , vertices[(*iter).vertices[i]].Points[2]);
			}
		}
	
		glEnd();

		glPopMatrix();
		
	glEndList();


}

void DrawableObj::Draw(float rotateX, float rotateY, float rotateZ, float scale)
{
	glPushMatrix();
	glTranslatef(0, -_yMin, 0);
	glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
	glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
	glScalef(scale, scale, scale);
	glCallList(_listNum);
	glPopMatrix();
}

DrawableObj::~DrawableObj()
{
	if (_listNum > 0)
		glDeleteLists(_listNum, 1);
}
