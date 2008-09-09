#pragma once

#include <Windows.h>

#include <cmath>

#include <vector>
#include <map>

#include "boost/tuple/tuple.hpp"

#include "Math.h"

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
		std::list<GameObject_ptr> ObjectsList;
		std::map<Math::Point3D, QbertBox_ptr> BoxMap;
	};
			
	struct EnemiesAppearanceData
	{
		std::string Name, Type;
		DWORD AppearanceFrequency, TimeSinceLastAppearance, MoveLength;
		AIFunction Function;
		std::vector<AppearanceBox> AppearanceBoxes;

		EnemiesAppearanceData (const std::string& type, const std::string& name, DWORD appearanceFrequency, DWORD moveLength,
			AIFunction function, std::vector<AppearanceBox> appearanceBoxes)
			: Name(name), Type(type), AppearanceFrequency(appearanceFrequency), MoveLength(moveLength), TimeSinceLastAppearance(0),
			Function(function), AppearanceBoxes(appearanceBoxes) {}
		EnemiesAppearanceData() {}
	};

protected:
	bool _isQbertAlive, _isFirstCall;
	int _boxesUnvisited;
	float _freeFallAcceleration;
	std::string _visitedBoxName, _unvisitedBoxName;
	QbertBox_ptr _startingBox;
	ModelObjects _objects;
	std::list<EnemiesAppearanceData> _enemiesAppearanceData; //The point3D is upDirection.

	void VisitBox (QbertBox_ptr box);
	void InsertBox (Math::Point3D point, QbertBox_ptr box);
	void SetQbert (std::string qbertName, QbertBox_ptr box);

	void SetEnemysFunction (QbertEnemyObj_ptr enemy, AIFunction function) {enemy->_AIfunc = function;}
	void SetEnemysMoveLength (QbertEnemyObj_ptr enemy, DWORD MoveLength) {enemy->_moveLength = MoveLength;}

public:
	QbertModel (std::string boxNameBefore, std::string boxNameAfter, float freeFallAcceleration) :
	  _unvisitedBoxName(boxNameBefore), _visitedBoxName(boxNameAfter), _isQbertAlive(true), _freeFallAcceleration(freeFallAcceleration) ,
		  _boxesUnvisited(0), _isFirstCall(false) {}

	QbertModel::ModelObjects& GetModelObjects() {return _objects;}
	virtual void Move(QbertGameObject_ptr ,const SimpleControler::InputData& inputdata) = 0;
	virtual void MoveEnemies(DWORD deltaTime) = 0;
	virtual void AddNewEnemyType(const std::string& type, const std::string& name, AIFunction function, DWORD appearanceFrequency, 
		DWORD moveLength, std::vector<AppearanceBox> appearanceBoxes) = 0;
	virtual void CreateEnemies (DWORD deltaTime) = 0;
	virtual void ReciveInput(const SimpleControler::InputData&) = 0;
	bool IsQbertAlive() {return _isQbertAlive;}
	bool IsGameWon() {return _boxesUnvisited == 0;}
	QbertBox_ptr GetBoxAt(const Math::Point3D&);
	QbertBox_ptr GetBoxAt(int x, int y, int z);
};

}	//namespace BGComplete
