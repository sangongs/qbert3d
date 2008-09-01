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

class QbertModel : public Model
{
	friend class QbertEnemyObj;

public:
	class ModelObjects
	{
		friend class QbertModel;
	
	public:
		QbertGameObject_ptr Qbert;
		std::list<QbertEnemyObj_ptr> Enemies;
		std::list<QbertBox_ptr> Boxes;
		std::map<Point3D, QbertBox_ptr> BoxesMap;
		std::list<GameObject_ptr> ObjectsList;
	};

protected:
	float _freeFallAcceleration;
	bool _isQbertAlive, _isFirstCall;
	std::string _boxNameAfter, _boxNameBefore;
	QbertBox_ptr _startingBox;
	int _boxesUnvisited;
	ModelObjects _objects;

	std::map<std::string, AIFunction> _AITypes;
	std::map<std::string, DWORD> _enemiesMoveLengthes;


	void VisitBox (QbertBox_ptr box);
	void InsertBox (Point3D point, QbertBox_ptr box);
	void SetQbert (std::string qbertName, QbertBox_ptr box);

public:
	QbertModel (std::string boxNameBefore, std::string boxNameAfter, float freeFallAcceleration) :
	  _boxNameBefore(boxNameBefore), _boxNameAfter(boxNameAfter), _isQbertAlive(true), _freeFallAcceleration(freeFallAcceleration) ,
		  _boxesUnvisited(0), _isFirstCall(false) {}

	QbertModel::ModelObjects& GetModelObjects() {return _objects;}
	virtual void Move(QbertGameObject_ptr ,const SimpleControler::InputData& inputdata) = 0;
	virtual void MakeEnemiesMove(DWORD deltaTime) = 0;
	virtual void ReciveInput(const SimpleControler::InputData&) = 0;
	bool IsQbertAlive() {return _isQbertAlive;}
	bool IsGameWon() {return _boxesUnvisited == 0;}
};
