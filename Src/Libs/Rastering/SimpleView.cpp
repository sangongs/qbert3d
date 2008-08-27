#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "boost\shared_ptr.hpp"

#include "GameObject.h"

#include "SimpleView.h"

#include "QbertModel.h"


SimpleView::SimpleView()
{
	_x = 0;
	_y = 0;
	_z = 0;
	_xRotate = 0;
	_yRotate = 0;
	_zRotate = 0;
}

void SimpleView::SetGameObjects(std::list<GameObject_ptr> *objList)
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
	glPushMatrix();
	
 	glTranslatef(_x, _y, _z);
 	glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);
 	glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
 	glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);

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
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	glPushMatrix();
	glMatrixMode( GL_MODELVIEW );
	glEnable(GL_NORMALIZE);

	_objects.insert(std::pair<std::string, DrawableObj_Ptr>("object__2", DrawableObj_Ptr(new DrawableObj("D:\\Programing\\qbert3d\\Objects", "smurf_2.obj", "object__2", 2, 0, 0, 0))));
}

void SimpleView::Draw(bool clearScreen = true)
{
	if (clearScreen)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float lightParams[] = {0.0f, 0.0f, 100.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
	}

	for (std::list<GameObject_ptr>::const_iterator iter = _objList->begin(); iter != _objList->end(); iter++)
	{	
		glLoadIdentity();
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();
		glTranslatef((*iter)->X, (*iter)->Y, (*iter)->Z);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f);
	}

	SDL_GL_SwapBuffers();
}

void SimpleView::Draw(QbertModel::ModelObjects& modelObjects)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightParams[] = {0.0f, 0.0f, -10.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);




	for (std::list<QbertModel::QbertBox_ptr>::const_iterator iter = modelObjects.Boxes.begin(); iter != modelObjects.Boxes.end(); iter++)
	{	
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();
		glTranslatef((*iter)->X, (*iter)->Y, (*iter)->Z);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f);
	}


	for (std::list<QbertModel::QbertEnemyObj_ptr>::iterator iter = modelObjects.Enemies.begin(); iter != modelObjects.Enemies.end(); iter++)
	{
		Point3D center((*iter)->NowOn->X, (*iter)->NowOn->Y, (*iter)->NowOn->Z);
		center += (*iter)->NowOn->GetUpDirection();

		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();
		glTranslatef(center.Points[0], center.Points[1], center.Points[2]);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f);
	}

	Point3D center(modelObjects.Qbert->NowOn->X, modelObjects.Qbert->NowOn->Y, modelObjects.Qbert->NowOn->Z);
	center += modelObjects.Qbert->NowOn->GetUpDirection(); 

	std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find(modelObjects.Qbert->Name);
	if (objToDraw == _objects.end())
		throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
	glLoadIdentity();
	glTranslatef(center.Points[0], center.Points[1], center.Points[2]);
	(*objToDraw).second->Draw(modelObjects.Qbert->XRotate, modelObjects.Qbert->YRotate, modelObjects.Qbert->ZRotate, 1.0f);

	SDL_GL_SwapBuffers();
}

void SimpleView::ChangeResolution(unsigned /*width*/, unsigned /*height*/)
{

}
