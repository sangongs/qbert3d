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
		static const float _floatEquPrecision;
		static const float _point3DEquPrecision;
		std::map<std::string, DrawableObj_Ptr> _objects;

	protected:
		QbertModel::ModelObjects_Ptr _modelToDraw;
		virtual void SetUpCamera() = 0;
		virtual void SetupLights();
		virtual void PerformAdditionalTransformations (const GameObject_ptr& obj, bool inverted);

		void AddNewObject (const std::string&, const std::string&, const std::string&, float, float, float, float);

	public:
		SimpleQbertView() {}

		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey) = 0;
		virtual void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
		virtual void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		virtual void DrawObj(const GameObject_ptr& obj, bool isBox = true);
		virtual void Init();
		void operator =(SimpleQbertView&);
	};
}
