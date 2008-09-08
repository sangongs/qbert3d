#pragma once

#include <map>

#include "View.h"
#include "DrawableObj.h"

#include "QbertModel.h"

namespace BGComplete
{
	class ArielView : public View
	{
	private:
		float _x, _y, _z, _xRotate, _yRotate, _zRotate;
		std::map<std::string, DrawableObj_Ptr> _objects;
		std::list<GameObject_ptr> *_objList;

	protected:
		unsigned _width, _height;
		virtual void SetUpCamera(QbertModel::ModelObjects& modelObjects);
		virtual void SetupLights(QbertModel::ModelObjects& modelObjects);
		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate);
		virtual void PerformAdditionalTransformations (const GameObject_ptr& obj, bool inverted);

	public:
		ArielView();
		void ChangeResolution(unsigned width, unsigned height);
		void Draw(QbertModel::ModelObjects&);
		void Draw(bool clearScreen);
		void DrawObj(const GameObject_ptr& obj, bool isBox = true);
		void Init(unsigned width, unsigned height);
		void SetGameObjects(std::list<GameObject_ptr> *objList);
	};
}
