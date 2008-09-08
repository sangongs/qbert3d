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

#include "Point3D.h"

#include "DrawableObj.h"


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
	Face(std::string arguments, const std::string& mtlObjName) : MtlObjName(mtlObjName)
	{
		int count = 0, v, t, n;
		arguments += " ";
		while (sscanf(arguments.c_str(), "%d/%d/%d", &v, &t, &n) == 3)
		{
			vertices.resize(++count);
			texture.resize(count);
			normals.resize(count);
			vertices[count - 1] = v;
			texture[count - 1] = t;
			normals[count - 1] = n;
			size_t search = arguments.find(' ', 1);
			if (search == arguments.npos)
				break;
			else
				arguments = arguments.substr(search, arguments.length() - search);
		}
		
		for (unsigned i = 0; i < vertices.size(); i++)
		{
			vertices[i]--;
			texture[i]--;
			normals[i]--;
		}
	}

	Face(){};

	std::vector<unsigned> vertices, texture, normals;
	std::string MtlObjName;
};


GLuint LoadTexture(const std::string& file)
{
	SDL_Surface *surface;
	if (!(surface = SDL_LoadBMP(file.c_str())))
		throw std::exception("Couldn't load bitmap file");

	GLint nOfColors = surface->format->BytesPerPixel;
	GLenum texture_format;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} 
	else if (nOfColors == 3)     // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	} 
	else 
		throw std::exception("Image is not truecolor, can't load it.");

	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	SDL_FreeSurface( surface );
	return texture;
}

class MtlObj
{
public:
	MtlObj() : HasTexture(false){}

	Point3D ka, kd, ks;
	int illum;
	int ns;
	bool HasTexture;
	GLuint Texture;
};

float DSin(float degrees)
{
	return (float)sin(degrees * (M_PI / 180));
}

float DCos(float degrees)
{
	return (float)cos(degrees * (M_PI / 180));
}

float Dacos(float value)
{
	return (float)(acos(value) * (180 / M_PI));
}

float Modulu(float inp)
{
	return inp - floor(inp);
}

std::string ReadMultipleLines(std::ifstream& inp)
{
	if (inp.eof())
		return "";

	char buffer[1024];
	inp.getline(buffer, 1024);
	std::string read(buffer);
	if (!read.empty() && (read.substr(read.length() - 1, 1) == "\\"))
		return read.substr(0, read.length() - 1) + " " + ReadMultipleLines(inp);
	else
		return read;
}

DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, float scale, float rotateX, float rotateY, float rotateZ) : _listNum(-1) // so that we wont accedently delete a list
{
	std::string string((boost::format("%1%\\%2%") % directory % fileName).str().c_str());
	std::ifstream objFile((boost::format("%1%\\%2%") % directory % fileName).str().c_str());
	
	if (!objFile.is_open())
		throw std::exception("Cant open obj file");

	std::string currentMtllib;
	std::string mtlObjName;
	
	std::vector<Point3D> vertices(0);
	std::vector<Point2D> texturePoints(0);
	std::vector<Point3D> normals(0);
	std::list<Face> faces;

	while (!objFile.eof())
	{
		std::string line(ReadMultipleLines(objFile));
		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if (command == "mtllib")
			currentMtllib = arguments;
		else if (command == "v")
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

	objFile.close();

	if (faces.size() == 0)
		throw std::exception("Couldn't find object in file when trying to construct DrawableObj");

	std::ifstream mtlFile((boost::format("%1%\\%2%") % directory % currentMtllib).str().c_str());

	if (!mtlFile.is_open())
		throw std::exception("Cant open obj's mtl file");

	std::string currentMtlObjName;
	std::map<std::string, MtlObj> mtlObjects;
	MtlObj currentMtlObj;

	while (!mtlFile.eof())
	{
		std::string line = ReadMultipleLines(mtlFile);

		size_t commandEnd(line.find(' ', 0));
		std::string command = line.substr(0, commandEnd);
		std::string arguments = line.substr(commandEnd + 1, line.npos);

		if ((command == "newmtl") || (command.empty()))
		{		
			if (!currentMtlObjName.empty())
				mtlObjects.insert(std::pair<std::string, MtlObj>(currentMtlObjName, currentMtlObj));

			currentMtlObj = MtlObj();
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
		else if ((command == "map_Kd") && (!arguments.empty()))
		{
			currentMtlObj.Texture = LoadTexture((boost::format("%1%\\%2%") % directory % arguments).str().c_str());
			currentMtlObj.HasTexture = true;
		}
	}

	mtlFile.close();

	float sumOfMass[3] = {0, 0, 0};
	
	for (size_t v = 0; v < vertices.size(); v++)
		for (int i = 0; i < 3; i++)
			sumOfMass[i]+= vertices[v].Points[i];

	for (int i = 0; i < 3; i++)
		sumOfMass[i] /= vertices.size();

	float maxPoint[3] = {0, 0, 0}, minPoint[3] = {0, 0, 0};

	for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		for (unsigned i = 0; i < (*iter).vertices.size(); i++)
		{
			float a = DCos(rotateX), c = DCos(rotateY), e = DCos(rotateZ);
			float b = DSin(rotateX), d = DSin(rotateY), f = DSin(rotateZ);
			float x = vertices[(*iter).vertices[i]].Points[0] - sumOfMass[0];
			float y = vertices[(*iter).vertices[i]].Points[1] - sumOfMass[1];
			float z = vertices[(*iter).vertices[i]].Points[2] - sumOfMass[2];
			float nx = e * c * x + (f * a + e * d * b) * y + (f * b - e * d * a) * z;
			float ny = (-f * c) * x + (e * a - f * d * b) * y + (e * b - f * d * b) * z;
			float nz = d * x + (-b * c) * y + a * c * z;
			
			maxPoint[0] = std::max(maxPoint[0], nx);
			maxPoint[1] = std::max(maxPoint[1], ny);
			maxPoint[2] = std::max(maxPoint[2], nz);

			minPoint[0] = std::min(minPoint[0], nx);
			minPoint[1] = std::min(minPoint[1], ny);
			minPoint[2] = std::min(minPoint[2], nz);
		}
	
	scale /= std::max(std::max(maxPoint[0] - minPoint[0], maxPoint[1] - minPoint[1]), maxPoint[2] - minPoint[2]);

	_yDistFromFloor = minPoint[1] * scale;

	if (0 == (_listNum = glGenLists(1)))
		throw std::exception("Can't generate display list while creating a drawable object.");

	glNewList(_listNum, GL_COMPILE);
		
		glMatrixMode(GL_MODELVIEW);
		glScalef(scale, scale, scale);
		glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
		glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
		glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
		glTranslatef(-sumOfMass[0], -sumOfMass[1], -sumOfMass[2]);

		for (std::list<Face>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
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


			if ((*mtlObjIter).second.HasTexture)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, (*mtlObjIter).second.Texture);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glBegin(GL_POLYGON);

			for (unsigned i = 0; i < (*iter).vertices.size(); i++)
			{
				normals[(*iter).normals[i]].Normalize();
				glNormal3fv(normals[(*iter).normals[i]].Points);

				if ((*mtlObjIter).second.HasTexture)
					glTexCoord2f(Modulu(texturePoints[(*iter).texture[i]].Points[0]), Modulu(texturePoints[(*iter).texture[i]].Points[1]));
				
				glVertex3f(vertices[(*iter).vertices[i]].Points[0]
						 , vertices[(*iter).vertices[i]].Points[1]
						 , vertices[(*iter).vertices[i]].Points[2]);
			}

			glEnd();
		}
	
		glPopMatrix();
	glEndList();
}

void DrawableObj::Draw(float rotateX, float rotateY, float rotateZ, float scale)
{
	glPushMatrix();

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