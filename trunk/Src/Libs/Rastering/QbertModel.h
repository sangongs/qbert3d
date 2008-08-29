#pragma once

#include <Windows.h>

#include <vector>
#include <map>

#include "DrawableObj.h"
#include "Model.h"
#include "SimpleControler.h"

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
		QbertBox(const std::string& name, int face, 
			float x, float y, float z, float xRotate = 0, float yRotate = 0, float zRotate = 0)
			: GameObject(name, x, y, z, xRotate, yRotate, zRotate), _face(face), _isVisited(false) {}
	};

	typedef boost::shared_ptr<QbertBox> QbertBox_ptr;


	class QbertGameObject : public GameObject
	{
	public:
		float Progress;
		DWORD MoveLength;
		QbertBox * NowOn, * AfterOn;
		Point3D UpDirection, FaceDirection;
		Point3D RotationAxe;
		Point3D RotationAngle;

		QbertGameObject(const std::string& name ="", QbertBox* box = NULL, DWORD moveLength = 1)
			: GameObject(name, 0, 0, 0, 0, 0, 0), NowOn(box), AfterOn(box), Progress(0), MoveLength(moveLength) 
		{}

		const QbertBox& WhereNow() const;
		const QbertBox& WhereNext() const;

		~QbertGameObject() {}
	};

	typedef boost::shared_ptr<QbertGameObject> QbertGameObject_ptr;


	class QbertEnemyObj : public QbertGameObject
	{
		std::string _type;
		bool _isFuncInit, _isMoveLengthInit;
		AIFunction _AIfunc;
	public:
		QbertEnemyObj(const std::string& name ="", QbertBox* box = NULL, const std::string& type ="", DWORD moveLegth = 1) 
			: QbertGameObject(name, box, moveLegth), _isFuncInit(false), _isMoveLengthInit(false), _type(type) {};

		void Move(QbertModel& model, DWORD deltaTime)
		{
			if (!_isMoveLengthInit)
			{
				MoveLength = model._enemiesMoveLengthes.find(_type)->second;
				_isMoveLengthInit = true;
			}

			if (!_isFuncInit)
			{
				_AIfunc = model._AITypes.find(_type)->second;
				_isFuncInit = true;
			}

			Progress += (float)(deltaTime / MoveLength);
			if (Progress > 1 )
			{

				Direction direction = _AIfunc(model);

				//[todo] write implematation;

				while (Progress > 1)
					Progress--;
			}

			if (!model._isQbertAlive)
				return;

			if (NowOn == model._objects.Qbert->AfterOn && AfterOn == model._objects.Qbert->NowOn)
			{
				if (Progress + model._objects.Qbert->Progress > 1)
					model._isQbertAlive = false;
			}
			else if (model._objects.Qbert->Progress > 0.3 && Progress > 0.8 && AfterOn == model._objects.Qbert->NowOn)
				model._isQbertAlive = false;
		}

		~QbertEnemyObj(void);
	};

	typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;


	class ModelObjects
	{
		friend class ModelObjectsIters;
	public:
		QbertGameObject_ptr Qbert;
		std::list<QbertEnemyObj_ptr> Enemies;
		std::list<QbertBox_ptr> Boxes;
		std::map<Point3D, QbertBox_ptr> BoxesMap;
		std::list<GameObject_ptr> ObjectsList;

		void SetQbertList() 
		{
			_qbertList = std::list<QbertGameObject_ptr>();
			_qbertList.push_back(Qbert);
		}

	private:
		std::list<QbertGameObject_ptr> _qbertList;					//for the Iterator;
	};

protected:
	bool _isQbertAlive, _isFirstCall;
	std::string _boxNameAfter, _boxNameBefore;
	QbertBox* _startingBox;
	int _boxesUnvisited;

	ModelObjects _objects;

	std::map<std::string, AIFunction> _AITypes;
	std::map<std::string, DWORD> _enemiesMoveLengthes;


	void VisitBox (QbertBox& box)
	{
		if (box._isVisited)
			return;
		
		box._isVisited = true;
		_boxesUnvisited--;
	}

	void VisitBox (QbertBox* box)
	{
		if (box->_isVisited)
			return;

		box->_isVisited = true;
		_boxesUnvisited--;
	}

	void InsertBox (Point3D point, QbertBox_ptr box)
	{
		_objects.BoxesMap.insert(std::pair<Point3D, QbertBox_ptr>(point, box));
		_objects.Boxes.push_back(box);
	}

	void SetQbert (std::string qbertName, QbertBox* box)
	{
		_objects.Qbert = QbertGameObject_ptr(new QbertGameObject(qbertName, box));
		_objects.SetQbertList();
	}

public:
	QbertModel (std::string boxNameBefore, std::string boxNameAfter) : _boxNameBefore(boxNameBefore), _boxNameAfter(_boxNameAfter), _isQbertAlive(true), _boxesUnvisited(0), _isFirstCall(false) {}

	QbertModel::ModelObjects& GetModelObjects() {return _objects;}
	virtual void QbertMove(const SimpleControler::InputData& inputdata) = 0;
	virtual void MakeEnemiesMove(DWORD deltaTime) = 0;
	virtual void ReadInput(const SimpleControler::InputData&) = 0;		//Deals with time and decides whether to move or not.
	bool IsQbertAlive() {return _isQbertAlive;}
	bool IsGameWon() {return _boxesUnvisited == 0;}
};
