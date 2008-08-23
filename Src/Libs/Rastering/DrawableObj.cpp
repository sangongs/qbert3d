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
	Point3D(const std::string& arguments)
	{
		LoadFromArguments(arguments);
	}

	LoadFromArguments(const std::String arguments)
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
	Face(const std::string &arguments, const std::string& mtlObjName)
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
	std::string mtlObjName;
};

class MtlObj
{
public:
	MtlObj();
	Point3D ka, kd, ks;
	int illum;
	int ns;

	

};

std::String readLine(ifstream& stream)
{
	char buffer[1024];
	stream.getline(buffer, 1024);
	return std::string(buffer);
}

DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, const std::string &objName, float scale) : _listNum(-1) // so that we wont accedently delete a list
{
	std::ifstream objFile((directory + "\\" + fileName).c_str);

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

	ifstream mtlFile((directory + "\\" + fileName).c_str());

	std::string currentMtlObjName;
	map<std::string, MtlObj> mtlObjects;
	MtlObj currentMtlObj;

	while (!objFile.eof())
	{
		std::string line(readLine(mtlFile));
		
		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if (command == "newmtl")
		{		
			if (!currentMtlObjName.empty())
				map.insert(std::pair<std::string, MtlObj>(currentMtlObjName, currentMtlObj));

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

	if (0 == (_listNum = glGenLists(1)))
		throw std::exception("Can't generate display list while creating a drawable object.");

	glNewList(_listNum, GL_COMPILE);
		glBegin(GL_TRIANGLES);

		for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		{
			for (int i = 0; i < 3; i++)
			{
				normals[(*iter).normals[i]].Normalize();
				
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v, 0);
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
