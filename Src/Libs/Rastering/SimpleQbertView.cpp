#include "StdAfx.h"

#include "boost\shared_ptr.hpp"
#include "boost\foreach.hpp"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Point3D.h"
#include "MathUtils.h"
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
	const float SimpleQbertView::_floatEquPrecision = 0.001f;
	const float SimpleQbertView::_point3DEquPrecision = 0.003f;

	void SimpleQbertView::AddNewObject(const std::string& name, const std::string& folder, const std::string& fileName, float scale, float xRotate, float yRotate, float zRotate)
	{
		_objects.insert(std::pair<std::string, DrawableObj_Ptr>(name, DrawableObj_Ptr(new DrawableObj(folder, fileName, scale, xRotate, yRotate, zRotate))));
	}

	void SimpleQbertView::Init()
	{
		AddNewObject("Qbert", "Objects", "qbert_3.obj", 1, -90, 0, 0);
		AddNewObject("RedBox", "Objects", "cube.obj", 1, 0, 0, 0);
		AddNewObject("BlueBox", "Objects", "cube2.obj", 1, 0, 0, 0);
		AddNewObject("ball", "Objects", "vall.obj", 1, -90, -100, 0);
		AddNewObject("directEnemy", "Objects", "smurf_2.obj", 1, -90, 0 , 0);
		AddNewObject("chaser", "Objects", "toad.obj", 1, -90, -100, 0);
	}

	void SimpleQbertView::SetupLights()
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float lightParams[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
	}

	void SimpleQbertView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		_modelToDraw = modelObjects;
	}

	void SimpleQbertView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (_modelToDraw->gameStage != GoingOn)
			return;

		glViewport(startX, startY, width, height );
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

		if (clearAndSwap)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		SetupLights();	
		SetUpCamera();

		BOOST_FOREACH(GameObject_ptr iter, _modelToDraw->Boxes)
			DrawObj(iter);

		BOOST_FOREACH(GameObject_ptr iter, _modelToDraw->Enemies)
			DrawObj(iter, false);

		DrawObj(boost::dynamic_pointer_cast<GameObject>(_modelToDraw->Qbert), false);
		
		if (clearAndSwap)
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

		glScalef(obj->Scale, obj->Scale, obj->Scale);
	
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
		QbertGameObject_ptr temp = boost::static_pointer_cast<QbertGameObject>(obj);
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
