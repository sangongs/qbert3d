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

class Face
{
public:
	Face(const std::string &arguments, const std::string& mtlObjName) : MtlObjName(mtlObjName)
	{
		char temp[1024];
		if (sscanf(arguments.c_str(), "%s %s %s %s", temp, temp, temp, temp) == 3)
			vertices.resize(3);
		else
			vertices.resize(4);

		texture.resize(vertices.size());
		normals.resize(vertices.size());

		if (vertices.size() == 3)
		{
			if (sscanf(arguments.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d"
				, &vertices[0], &texture[0], &normals[0]
				, &vertices[0] + 1, &texture[0] + 1, &normals[0] + 1
				, &vertices[0] + 2, &texture[0] + 2, &normals[0] + 2) != 9)
				throw std::exception("Can't parse argument in constructor for face");
		}
		else
		{
			if (sscanf(arguments.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d"
				, &vertices[0], &texture[0], &normals[0]
				, &vertices[0] + 1, &texture[0] + 1, &normals[0] + 1
				, &vertices[0] + 2, &texture[0] + 2, &normals[0] + 2
				, &vertices[0] + 3, &texture[0] + 3, &normals[0] + 3) != 12)
				throw std::exception("Can't parse argument in constructor for face");
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
	if ( !(surface = SDL_LoadBMP(file.c_str())) ) 
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
		else if (command == "v")
			vertices.push_back(Point3D(arguments));
		else if (command == "vt")
			texturePoints.push_back(Point2D(arguments));
		else if (command == "vn")
			normals.push_back(Point3D(arguments));
		
		if (command == "usemtl")
			mtlObjName = arguments;
		else if (command == "f")
			faces.push_back(Face(arguments, mtlObjName));

		if (readingObject)
		{
			
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
		else if ((command == "map_Kd") && (!arguments.empty()))
		{
			currentMtlObj.Texture = LoadTexture((boost::format("%1%\\%2%") % directory % arguments).str().c_str());
			currentMtlObj.HasTexture = true;
		}
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
		for (unsigned i = 0; i < (*iter).vertices.size(); i++)
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
		for (unsigned i = 0; i < (*iter).vertices.size(); i++)
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
			}

			if (vertices.size() == 3)
				glBegin(GL_TRIANGLES);
			else
				glBegin(GL_QUADS);

			for (unsigned i = 0; i < (*iter).vertices.size(); i++)
			{
				normals[(*iter).normals[i]].Normalize();
				glNormal3fv(normals[(*iter).normals[i]].Points);
				
				if ((*mtlObjIter).second.HasTexture)
					glTexCoord2f(texturePoints[(*iter).texture[i]].Points[0], texturePoints[(*iter).texture[i]].Points[1]);
				
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
