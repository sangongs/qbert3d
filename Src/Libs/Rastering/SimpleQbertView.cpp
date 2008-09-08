#include "StdAfx.h"

#include "boost\shared_ptr.hpp"
#include "boost\foreach.hpp"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Point3D.h"
#include "Math.h"
#include "DrawableObj.h"
#include "GameObject.h"

#include "QbertModel.h"
//maybe put the header include here...

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

#include "SimpleQbertView.h"

namespace BGComplete
{
	void SimpleQbertView::Init()
	{
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("Qbert", DrawableObj_Ptr(new DrawableObj("Objects", "smurf_2.obj", 1, -90, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("RedBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube.obj", 1, 0, 0, 0))));
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>("BlueBox", DrawableObj_Ptr(new DrawableObj("Objects", "cube2.obj", 1, 0, 0, 0))));
	}

	void SimpleQbertView::SetupLights(QbertModel::ModelObjects& /*modelObjects*/)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float lightParams[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
	}

	void SimpleQbertView::Draw(QbertModel::ModelObjects& modelObjects, bool clear, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		glViewport(startX, startY, width, height );
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //[todo] understand what this does.
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

		if (clear)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

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

	void SimpleQbertView::DrawObj(const GameObject_ptr& obj, bool isBox)
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

	void ChangeCoordinateSystem(std::pair<Math::Point3D, Math::Point3D> from, std::pair<Math::Point3D, Math::Point3D> to, float floatEquPrecision, float point3DEquPrecision, bool inverted, bool validate)
	{
		from.first.Normalize();
		from.second.Normalize();
		to.first.Normalize();
		to.second.Normalize();

		if (validate)
		{
			float firstScalar = from.first.ScalarProduct(from.second), secondScalar = to.first.ScalarProduct(to.second);

			if (firstScalar == -1 || secondScalar == -1 || std::abs(firstScalar - secondScalar) > floatEquPrecision)
				throw std::exception("Incompatible set of coordinate systems, can't match them (in function View::ChangeSystemCoordinate()");

			if (firstScalar)
			{
				from.second = (from.first.CrossProduct(from.second)).CrossProduct(from.first);
				to.second = (to.first.CrossProduct(to.second)).CrossProduct(to.first);
			}
		}

		Math::Point3D rotationDirection = 0.5 * (from.first + to.first);
		if (rotationDirection.IsEqual(Math::Point3D::Zero, point3DEquPrecision))
			rotationDirection = to.second;

		Math::Point3D translatedSecondVector = (2 * (rotationDirection.ScalarProduct(from.second) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
			from.second).Normalize();

		float rotationAngle = Math::Dacos(translatedSecondVector.ScalarProduct(to.second));
		Math::Point3D normalVector = translatedSecondVector.CrossProduct(to.second);
		if (!normalVector.IsEqual(Math::Point3D::Zero, point3DEquPrecision) && !normalVector.Normalize().IsEqual(to.first, point3DEquPrecision))
			rotationAngle *= -1;

		if (inverted)
		{
			glRotatef(-180.0f, rotationDirection.X(), rotationDirection.Y(), rotationDirection.Z());
			glRotatef(-rotationAngle, to.first.X(), to.first.Y(), to.first.Z());
		}
		else
		{
			glRotatef(rotationAngle, to.first.X(), to.first.Y(), to.first.Z());
			glRotatef(180.0f, rotationDirection.X(), rotationDirection.Y(), rotationDirection.Z());
		}
	}

	void SimpleQbertView::PerformAdditionalTransformations(const GameObject_ptr &obj, bool inverted)
	{
		ChangeCoordinateSystem(
			std::pair<Math::Point3D, Math::Point3D>(
				Math::Point3D(0, 0, 1),
				Math::Point3D(0, 1, 0)),
			std::pair<Math::Point3D, Math::Point3D>(
				boost::static_pointer_cast<QbertGameObject>(obj)->CurrentFaceDirection,
				boost::static_pointer_cast<QbertGameObject>(obj)->CurrentUpDirection),
			_floatEquPrecision, _point3DEquPrecision, inverted, false);
	}

	void SimpleQbertView::operator=(SimpleQbertView& inp)
	{
		_objects = inp._objects;
	}
}