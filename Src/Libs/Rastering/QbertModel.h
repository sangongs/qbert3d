#pragma once

#include <Windows.h>

#include <cmath>

#include <vector>
#include <map>

#include "DrawableObj.h"
#include "Model.h"
#include "SimpleControler.h"

//[todo] remove all nested classes into different files.
class QbertModel : public Model
{
public:
	typedef Direction (*AIFunction)(QbertModel&);

	class QbertBox : public GameObject
	{
		friend QbertModel;

		int _face;
		bool _isVisited;
		bool _isOnPerimeter;
	public:
		QbertBox(const std::string& name, int face, bool isPerimeter,
			float x, float y, float z, float xRotate = 0, float yRotate = 0, float zRotate = 0)
			: GameObject(name, x, y, z, xRotate, yRotate, zRotate), _face(face), _isVisited(false), _isOnPerimeter(isPerimeter) {}

		bool IsOnPerimeter()
		{
			return _isOnPerimeter;
		}
	};
	typedef boost::shared_ptr<QbertBox> QbertBox_ptr;


	class QbertGameObject : public GameObject
	{
	protected:
		DWORD _moveLength;
		float _verticalSpeed, _horizontalSpeed, _freeFallAcceleration;

	public:
		float Progress;
		QbertBox_ptr LastBox, NextBox;
		Point3D LastUpDirection, LastFaceDirection, NextUpDirection, NextFaceDirection, CurrentUpDirection, CurrentFaceDirection;
		bool IsMovingUp, IsChangingBox, IsMoving;
		Direction MovingDirection;

		QbertGameObject(const std::string& name ="", QbertBox_ptr box = QbertBox_ptr(), DWORD moveLength = 1000, float freeFallAcceleration = 1)
			: GameObject(name, 0, 0, 0, 0, 0, 0), LastBox(box), NextBox(box), Progress(0), _moveLength(moveLength) , IsMoving(false), _freeFallAcceleration(freeFallAcceleration)
		{
			SetMoveLength(moveLength, _freeFallAcceleration);
		}

		~QbertGameObject() {}

		Point3D GetRightDirection() {return LastFaceDirection.CrossProduct(LastUpDirection);}
		DWORD GetMoveLength() {return _moveLength;}
		float GetHorizontalSpeed() {return _horizontalSpeed;}
		float GetVerticalSpeed(){return _verticalSpeed;}
		float GetFreeAcceleration(){return _freeFallAcceleration;}

		//[todo] into cpp file.
		void SetMoveLength(DWORD moveLength, float freeFallAcceleration)
		{
			_freeFallAcceleration = freeFallAcceleration;
			_moveLength = moveLength;
			_horizontalSpeed = 1;

			_verticalSpeed = std::abs(1.0f / _horizontalSpeed - 0.5f * _freeFallAcceleration * _horizontalSpeed);
		}
	};
	typedef boost::shared_ptr<QbertGameObject> QbertGameObject_ptr;


	class QbertEnemyObj : public QbertGameObject
	{
	private:
		std::string _type;
		bool _isFuncInit, _isMoveLengthInit;
		AIFunction _AIfunc;

	public:
		QbertEnemyObj(const std::string& name ="", QbertBox_ptr box = QbertBox_ptr(), const std::string& type ="", DWORD moveLegth = 100) 
			: QbertGameObject(name, box, moveLegth), _isFuncInit(false), _isMoveLengthInit(false), _type(type) {};

		void Move(QbertModel& model, DWORD deltaTime)//[todo] remove from header
		{
			if (!_isMoveLengthInit)
			{
				_moveLength = model._enemiesMoveLengthes.find(_type)->second;
				_isMoveLengthInit = true;
			}

			if (!_isFuncInit)
			{
				_AIfunc = model._AITypes.find(_type)->second;
				_isFuncInit = true;
			}


			model.Move(this, SimpleControler::InputData(deltaTime, _AIfunc(model)));

			if (!model._isQbertAlive)
				return;

			if (LastBox == model._objects.Qbert->NextBox && NextBox == model._objects.Qbert->LastBox)
			{
				if (Progress + model._objects.Qbert->Progress > 1)
					model._isQbertAlive = false;
			}
			else if (model._objects.Qbert->Progress > 0.3 && Progress > 0.8 && NextBox == model._objects.Qbert->LastBox)
				model._isQbertAlive = false;
		}

		~QbertEnemyObj(void);
	};
	typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;


	class ModelObjects
	{
	private:
		friend class QbertModel;
		float _freeFallAcceleration;
	
	public:
		QbertGameObject_ptr Qbert;
		std::list<QbertEnemyObj_ptr> Enemies;
		std::list<QbertBox_ptr> Boxes;
		std::map<Point3D, QbertBox_ptr> BoxesMap;
		std::list<GameObject_ptr> ObjectsList;

		float GetFreeFallAcceleration() {return _freeFallAcceleration;}
	};

protected:
	bool _isQbertAlive, _isFirstCall;
	std::string _boxNameAfter, _boxNameBefore;
	QbertBox_ptr _startingBox;
	int _boxesUnvisited;
	ModelObjects _objects;

	std::map<std::string, AIFunction> _AITypes;
	std::map<std::string, DWORD> _enemiesMoveLengthes;


	//[todo] remove code from here
	void VisitBox (QbertBox_ptr box)
	{
		if (box->_isVisited)
			return;
		
		box->_isVisited = true;
		box->Name = _boxNameAfter;
		_boxesUnvisited--;
	}

	void InsertBox (Point3D point, QbertBox_ptr box)
	{
		_objects.BoxesMap.insert(std::pair<Point3D, QbertBox_ptr>(point, box));
		_objects.Boxes.push_back(box);
	}

	void SetQbert (std::string qbertName, QbertBox_ptr box)
	{
		_objects.Qbert = QbertGameObject_ptr(new QbertGameObject(qbertName, box));
	}

	//until here

public:
	QbertModel (std::string boxNameBefore, std::string boxNameAfter, float FreeFallAcceleration) :
	  _boxNameBefore(boxNameBefore), _boxNameAfter(boxNameAfter), _isQbertAlive(true), 
		  _boxesUnvisited(0), _isFirstCall(false) {_objects._freeFallAcceleration = FreeFallAcceleration;}

	QbertModel::ModelObjects& GetModelObjects() {return _objects;}
	virtual void Move(QbertGameObject* ,const SimpleControler::InputData& inputdata) = 0;
	virtual void MakeEnemiesMove(DWORD deltaTime) = 0;
	virtual void ReadInput(const SimpleControler::InputData&) = 0;		//Deals with time and decides whether to move or not.
	bool IsQbertAlive() {return _isQbertAlive;}
	bool IsGameWon() {return _boxesUnvisited == 0;}
};
