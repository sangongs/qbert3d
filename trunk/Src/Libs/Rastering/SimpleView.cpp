#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "GameObject.h"

#include "SimpleView.h"

SimpleView::SimpleView()
{
	_objects.insert(pair<std::string, DrawableObj>("mouse", DrawableObj("D:\\Programing\\qbert3d\\Objects\\Mouse3.obj", "Mouse")));
}

SimpleView::~SimpleView(void)
{
}

void SimpleView::SetGameObjects(std::list<GameObject> *objList)
{
	_objList = objList;
}

void SimpleView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate)
{
	_x += deltaX;
	_y += deltaY;
	_z += deltaZ;
	_xRotate += xRotate;
	_yRotate += yRotate;
	_zRotate += zRotate;

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glTranslatef(_x, _y, _z);
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
}

void SimpleView::Init(unsigned int width, unsigned int height)
{
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //[todo] understand what this does.
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, width, height ); //[todo] understand what this does.
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glPushMatrix();
}

void SimpleView::Draw()
{	
	glColor3f(0.3f, 0.5f, 0.9f);
	glMatrixMode( GL_MODELVIEW )
	for (std::list<GameObject>::const_iterator iter = _objList->begin(); iter != _objList->end(); iter++)
	{
		std::map<std::string, DrawableObj>::const_iterator objToDraw = _objects.find((*iter).name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();
		glTranslatef((*iter).x, (*iter).y, (*iter).z);
		glTranslatef((*iter).xRotate, 1, 0, 0);
		glTranslatef((*iter).yRotate, 0, 1, 0);
		glTranslatef((*iter).zRotate, 0, 0, 1);

		(*objToDraw).second.Draw();
	}

	SDL_GL_SwapBuffers();
}
