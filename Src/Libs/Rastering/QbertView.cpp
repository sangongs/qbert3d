#include "StdAfx.h"

#include "boost\shared_ptr.hpp"
#include "boost\foreach.hpp"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Point3D.h"
#include "DrawableObj.h"
#include "GameObject.h"

#include "QbertModel.h"
#include "QbertView.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"


namespace BGComplete
{
	QbertView::QbertView()
	{

	}

	void QbertView::SetGameObjects(std::list<GameObject_ptr> *objList)
	{
		_objList = objList;
	}

	void QbertView::Init(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //[todo] understand what this does.
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glViewport( 0, 0, width, height );
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
		glMatrixMode( GL_MODELVIEW );
		

		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("Qbert", DrawableObj_Ptr(new DrawableObj("Objects", "smurf_2.obj", 1, -90, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("RedBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube.obj", 1, 0, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("BlueBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube2.obj", 1, 0, 0, 0))));

	}

	void QbertView::Draw(bool clearScreen = true) //[todo] figure out what to do with this.
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

	void QbertView::SetupLights(QbertModel::ModelObjects& /*modelObjects*/)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float lightParams[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
	}

	void QbertView::Draw(QbertModel::ModelObjects& modelObjects)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		SetupLights(modelObjects);	
		SetUpCamera(modelObjects);

		BOOST_FOREACH(GameObject_ptr iter, modelObjects.Boxes)
			DrawObj(iter);

		BOOST_FOREACH(GameObject_ptr iter, modelObjects.Enemies) //[todo] use boost::foreach everywhere!!! yeay! yum yum
			DrawObj(iter, false);

		DrawObj(boost::dynamic_pointer_cast<GameObject>(modelObjects.Qbert), false);

		SDL_GL_SwapBuffers();
	}

	void QbertView::DrawObj(const GameObject_ptr& obj, bool isBox)
	{
		std::map<std::string, DrawableObj_Ptr>::iterator objToDraw = _objects.find(obj->Name);
		if (objToDraw == _objects.end())
			throw std::exception("Couldn't find object while trying to draw the model, (in the view function)");

		glPushMatrix();
		glTranslatef(obj->Center.X(), obj->Center.Y(), obj->Center.Z());

		if (!isBox)
			PerformAdditionalTransformations(obj, false);

		(*objToDraw).second->Draw(obj->XRotate, obj->YRotate, obj->ZRotate, 1.0f);
		glPopMatrix();
	}

	void QbertView::PerformAdditionalTransformations(const GameObject_ptr &obj, bool inverted)
	{
		ChangeCoordinateSystem(std::pair<Math::Point3D, Math::Point3D>(Math::Point3D(0, 0, 1), Math::Point3D(0, 1, 0)),
			std::pair<Math::Point3D, Math::Point3D>(boost::static_pointer_cast<QbertGameObject>(obj)->CurrentFaceDirection, 
			boost::static_pointer_cast<QbertGameObject>(obj)->CurrentUpDirection), inverted, false);
	}

	void QbertView::ChangeResolution(unsigned /*width*/, unsigned /*height*/){} //[todo] implement this... maybe H should do it.!
}
