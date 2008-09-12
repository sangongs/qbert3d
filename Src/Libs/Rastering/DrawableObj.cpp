#include "StdAfx.h"

#include <stdlib.h>

#include <iostream>
#include <list>
#include <vector>
#include <exception>
#include <fstream>

#include <boost/format.hpp>

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "MathUtils.h"
#include "Point2D.h"
#include "Point3D.h"

#include "DrawableObj.h"


namespace BGComplete
{
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
		if ((surface = SDL_LoadBMP(file.c_str())) == NULL)
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

		Math::Point3D ka, kd, ks;
		int illum;
		int ns;
		bool HasTexture;
		GLuint Texture;
	};

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
		std::ifstream objFile((boost::format("%1%\\%2%") % directory % fileName).str().c_str());
		
		if (!objFile.is_open())
			throw std::exception("Cant open obj file");

		std::string currentMtllib;
		std::string mtlObjName;
		
		std::vector<Math::Point3D> vertices(0);
		std::vector<Math::Point2D> texturePoints(0);
		std::vector<Math::Point3D> normals(0);
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
				vertices.push_back(Math::Point3D(arguments));
			else if (command == "vt")
				texturePoints.push_back(Math::Point2D(arguments));
			else if (command == "vn")
				normals.push_back(Math::Point3D(arguments));
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

		Math::Point3D sumOfMass(0, 0, 0);
		
		BOOST_FOREACH(const Math::Point3D& vertix, vertices)
			sumOfMass += vertix;

		sumOfMass /= (float)vertices.size();

		float maxPoint[3] = {0, 0, 0}, minPoint[3] = {0, 0, 0};

		BOOST_FOREACH(const Face& iter, faces)
			for (unsigned i = 0; i < iter.vertices.size(); i++)
			{
				Math::Point3D center = vertices[iter.vertices[i]] - sumOfMass;

				float a = Math::DCos(rotateX), c = Math::DCos(rotateY), e = Math::DCos(rotateZ);
				float b = Math::DSin(rotateX), d = Math::DSin(rotateY), f = Math::DSin(rotateZ);

				float nx = center.ScalarProduct(e * c, f * a + e * d * b, f * b - e * d * a);
				float ny = center.ScalarProduct(-f * c, e * a - f * d * b, e * b - f * d * b);
				float nz = center.ScalarProduct(d, -b * c, a * c);
				
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

		glEnable(GL_NORMALIZE);
		glNewList(_listNum, GL_COMPILE);
			
			glMatrixMode(GL_MODELVIEW);
			glScalef(scale, scale, scale);
			glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
			glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
			glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
			glTranslatef(-sumOfMass[0], -sumOfMass[1], -sumOfMass[2]);

			BOOST_FOREACH(const Face& iter, faces)
			{
				std::map<std::string, MtlObj>::iterator mtlObjIter = mtlObjects.find(iter.MtlObjName);
				if (mtlObjIter == mtlObjects.end())
					throw std::exception("Couldn't find mtlObj for a face I was trying to draw.");

				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mtlObjIter->second.ka.Points);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mtlObjIter->second.kd.Points);

				if (mtlObjIter->second.illum == 2)
				{
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mtlObjIter->second.ks.Points);
					glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat)mtlObjIter->second.ns);
				}
				else
				{
					float zeroArray[] = {0.0f, 0.0f, 0.0f};
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroArray);
				}


				if (mtlObjIter->second.HasTexture)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, mtlObjIter->second.Texture);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				glBegin(GL_POLYGON);

				for (unsigned i = 0; i < iter.vertices.size(); i++)
				{
					normals[iter.normals[i]].Normalize();
					glNormal3fv(normals[iter.normals[i]].Points);

					if (mtlObjIter->second.HasTexture)
						glTexCoord2f(Math::Modulu(texturePoints[iter.texture[i]].Points[0]), Math::Modulu(texturePoints[iter.texture[i]].Points[1]));
					
					glVertex3f(vertices[iter.vertices[i]].Points[0]
							 , vertices[iter.vertices[i]].Points[1]
							 , vertices[iter.vertices[i]].Points[2]);
				}

				glEnd();
			}
		
			//glPopMatrix();
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
}
