#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "boost\shared_ptr.hpp"

#include "DrawableObj.h"
#include "GameObject.h"

#include "QbertModel.h"

#include "SimpleView.h"


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

	_objects.insert(std::pair<std::string, DrawableObj_Ptr>("Qbert", DrawableObj_Ptr(new DrawableObj("D:\\Programing\\qbert3d\\Objects", "smurf_2.obj", 1, -90, 0, 0))));
	_objects.insert(std::pair<std::string, DrawableObj_Ptr>("RedBox", DrawableObj_Ptr(new DrawableObj("D:\\Programing\\qbert3d\\Objects", "cube.obj", 1, 0, 0, 0))));
	_objects.insert(std::pair<std::string, DrawableObj_Ptr>("BlueBox", DrawableObj_Ptr(new DrawableObj("D:\\Programing\\qbert3d\\Objects", "cube2.obj", 1, 0, 0, 0))));

}

void SimpleView::Draw(bool clearScreen = true)
{
	if (clearScreen)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	for (std::list<GameObject_ptr>::const_iterator iter = _objList->begin(); iter != _objList->end(); iter++)
	{	
		glLoadIdentity();
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();
		glTranslatef((*iter)->X, (*iter)->Y, (*iter)->Z);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f, true);
	}

	SDL_GL_SwapBuffers();
}

void SimpleView::Draw(QbertModel::ModelObjects& modelObjects)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef(_x, _y, -_z);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightParams[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
	glLoadIdentity();

	for (std::list<QbertModel::QbertBox_ptr>::const_iterator iter = modelObjects.Boxes.begin(); iter != modelObjects.Boxes.end(); iter++)			//Drawing Boxes
	{	
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();

		glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);
		glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
		glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);		

		glTranslatef((*iter)->X, (*iter)->Y, (*iter)->Z);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f, false);
	}


	for (std::list<QbertModel::QbertEnemyObj_ptr>::iterator iter = modelObjects.Enemies.begin(); iter != modelObjects.Enemies.end(); iter++)		//Drawing Enemies
	{
		Point3D center((*iter)->NowOn->X, (*iter)->NowOn->Y, (*iter)->NowOn->Z);
		center += (*iter)->NowUpDirection;

		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find((*iter)->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
		glLoadIdentity();


		Point3D rotationDirection = (Point3D (0, 0, 1) + modelObjects.Qbert->NowFaceDirection) * 0.5;
		if ((rotationDirection.Points[0] == 0) && (rotationDirection.Points[1] == 0) && (rotationDirection.Points[2] == 0))
			rotationDirection = modelObjects.Qbert->NowUpDirection;

		Point3D tempUpDirection = (2 * (rotationDirection.ScalarProduct(modelObjects.Qbert->NowUpDirection) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
			modelObjects.Qbert->NowUpDirection).Normalize();

		glRotatef(DCos(tempUpDirection.ScalarProduct(modelObjects.Qbert->NowUpDirection)), 0, 0, 1);
		glRotatef(180, rotationDirection.X(), rotationDirection.Y(),rotationDirection.Z());


		glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);
		glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
		glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);	

		glTranslatef(center.Points[0], center.Points[1], center.Points[2]);
		(*objToDraw).second->Draw((*iter)->XRotate, (*iter)->YRotate, (*iter)->ZRotate, 1.0f, true);
	}

	Point3D center(modelObjects.Qbert->NowOn->X, modelObjects.Qbert->NowOn->Y, modelObjects.Qbert->NowOn->Z);										//Drawing Qbert
	center += modelObjects.Qbert->NowUpDirection; 

	std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find(modelObjects.Qbert->Name);
	if (objToDraw == _objects.end())
		throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
	glLoadIdentity();


	Point3D rotationDirection = 0.5 * (Point3D (0, 0, 1) + modelObjects.Qbert->NowFaceDirection);
	if ((rotationDirection.Points[0] == 0) && (rotationDirection.Points[1] == 0) && (rotationDirection.Points[2] == 0))
		rotationDirection = modelObjects.Qbert->NowUpDirection;

	Point3D tempUpDirection = (2 * (rotationDirection.ScalarProduct(modelObjects.Qbert->NowUpDirection) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
		modelObjects.Qbert->NowUpDirection).Normalize();

	glRotatef(DCos(tempUpDirection.ScalarProduct(modelObjects.Qbert->NowUpDirection)), 0, 0, 1);
	glRotatef(180, rotationDirection.X(), rotationDirection.Y(),rotationDirection.Z());

	glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);
	glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
	glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);	

	glTranslatef(center.Points[0], center.Points[1], center.Points[2]);
	(*objToDraw).second->Draw(modelObjects.Qbert->XRotate, modelObjects.Qbert->YRotate, modelObjects.Qbert->ZRotate, 1.0f, true);

	SDL_GL_SwapBuffers();
}

void SimpleView::ChangeResolution(unsigned /*width*/, unsigned /*height*/)
{

}
