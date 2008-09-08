#pragma once

#include <Windows.h>

#include <cmath>

#include <vector>
#include <map>

#include "DrawableObj.h"
#include "Model.h"
#include "SimpleControler.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

namespace BGComplete
{
	class QbertModel : public Model
	{
		friend class QbertEnemyObj;

	public:
		class ModelObjects
		{
		public:
			QbertGameObject_ptr Qbert;
			std::list<QbertEnemyObj_ptr> Enemies;
			std::list<QbertBox_ptr> Boxes;
			std::map<Math::Point3D, QbertBox_ptr> BoxMap;
			std::list<GameObject_ptr> ObjectsList;
		};

	protected:
		float _freeFallAcceleration;
		bool _isQbertAlive, _isFirstCall;
		std::string _visitedBoxName, _unvisitedBoxName;
		QbertBox_ptr _startingBox;
		int _boxesUnvisited;
		ModelObjects _objects;

		std::map<std::string, AIFunction> _AITypes;
		std::map<std::string, DWORD> _enemiesMoveLengthes;


		void VisitBox (QbertBox_ptr box);
		void InsertBox (Math::Point3D point, QbertBox_ptr box);
		void SetQbert (std::string qbertName, QbertBox_ptr box);

	public:
		QbertModel (std::string boxNameBefore, std::string boxNameAfter, float freeFallAcceleration) :
		  _unvisitedBoxName(boxNameBefore), _visitedBoxName(boxNameAfter), _isQbertAlive(true), _freeFallAcceleration(freeFallAcceleration) ,
			  _boxesUnvisited(0), _isFirstCall(false) {}

		QbertModel::ModelObjects& GetModelObjects() {return _objects;}
		virtual void Move(QbertGameObject_ptr ,const SimpleControler::InputData& inputdata) = 0;
		virtual void MoveEnemies(DWORD deltaTime) = 0;
		virtual void ReciveInput(const SimpleControler::InputData&) = 0;
		bool IsQbertAlive() {return _isQbertAlive;}
		bool IsGameWon() {return _boxesUnvisited == 0;}
	};
}
