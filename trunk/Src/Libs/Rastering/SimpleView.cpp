#include "StdAfx.h"

#include "boost\shared_ptr.hpp"
#include "boost\foreach.hpp"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Point3D.h"
#include "DrawableObj.h"
#include "GameObject.h"

#include "QbertModel.h"
#include "SimpleView.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"


namespace BGComplete
{
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

		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("Qbert", DrawableObj_Ptr(new DrawableObj("Objects", "smurf_2.obj", 1, -90, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("RedBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube.obj", 1, 0, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("BlueBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube2.obj", 1, 0, 0, 0))));

	}

	void SimpleView::Draw(bool clearScreen = true)
	{
		if (clearScreen)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
		}

		BOOST_FOREACH(GameObject_ptr iter, *_objList)
		{
			glLoadIdentity();
			std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find(iter->Name);
			if (objToDraw == _objects.end())
				throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");
			glLoadIdentity();

			glTranslatef(iter->Center.X(), iter->Center.Y(), iter->Center.Z());
			(*objToDraw).second->Draw(iter->XRotate, iter->YRotate, iter->ZRotate, 1.0f);
		}

		SDL_GL_SwapBuffers();
	}

	void SimpleView::SetupLights()
	{
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
	}

	void SimpleView::Draw(QbertModel::ModelObjects& modelObjects)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		SetupLights();

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		BOOST_FOREACH(GameObject_ptr iter, modelObjects.Boxes)
			DrawObj(iter);

		BOOST_FOREACH(GameObject_ptr iter, modelObjects.Enemies) //[todo] use boost::foreach everywhere!!! yeay! yum yum
			DrawObj(iter, false);

		DrawObj(boost::dynamic_pointer_cast<GameObject>(modelObjects.Qbert), false);


		SDL_GL_SwapBuffers();
	}

	void SimpleView::DrawObj(const GameObject_ptr& obj, bool isBox)
	{
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find(obj->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");

		glLoadIdentity();
		glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);
		glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
		glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);
		glTranslatef(obj->Center.X(), obj->Center.Y(), obj->Center.Z());

		if (!isBox)
			PerformAdditionalTransformations(obj);

		(*objToDraw).second->Draw(obj->XRotate, obj->YRotate, obj->ZRotate, 1.0f);
	}

	void SimpleView::PerformAdditionalTransformations(const GameObject_ptr &obj)
	{
		ChangeCoordinateSystem(std::pair<Math::Point3D, Math::Point3D>(Math::Point3D(0, 0, 1), Math::Point3D(0, 1, 0)),
			std::pair<Math::Point3D, Math::Point3D>(boost::static_pointer_cast<QbertGameObject>(obj)->CurrentFaceDirection, 
				boost::static_pointer_cast<QbertGameObject>(obj)->CurrentUpDirection), false);
	}

	void SimpleView::ChangeResolution(unsigned /*width*/, unsigned /*height*/){} //[todo] implement this... maybe H should do it.!
}
