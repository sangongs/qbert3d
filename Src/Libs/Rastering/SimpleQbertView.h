#pragma once

#include <map>

#include "QbertView.h"
#include "DrawableObj.h"

#include "QbertModel.h"

namespace BGComplete
{
	class SimpleQbertView : public QbertView
	{
	private:
		const float _floatEquPrecision;
		const float _point3DEquPrecision;
		std::map<std::string, DrawableObj_Ptr> _objects;

	protected:
		virtual void SetUpCamera(QbertModel::ModelObjects& modelObjects) = 0;
		virtual void SetupLights(QbertModel::ModelObjects& modelObjects);
		virtual void PerformAdditionalTransformations (const GameObject_ptr& obj, bool inverted);

	public:
		SimpleQbertView() : _floatEquPrecision(0.001f), _point3DEquPrecision(0.003f){};

		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, bool viewChange) = 0;
		virtual void Draw(QbertModel::ModelObjects& modelObjects, bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		virtual void DrawObj(const GameObject_ptr& obj, bool isBox = true);
		virtual void Init();
		void operator =(SimpleQbertView&);
	};
}
