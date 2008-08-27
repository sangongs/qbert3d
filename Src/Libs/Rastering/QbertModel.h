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
		Point3D _upDirection;
		QbertBox* _neibhors[5];
		bool _isVisited;
	public:
		Point3D GetUp();
		QbertBox(const std::string& name, const Point3D& upDirection, int face, 
			float x, float y, float z, float xRotate = 0, float yRotate = 0, float zRotate = 0)
			: GameObject(name, x, y, z, xRotate, yRotate, zRotate), _upDirection(upDirection), _face(face), _isVisited(false)
		{
			_neibhors[None] = this;
		}

		Point3D GetUpDirection()
		{

			return _upDirection;
		}
	};

	typedef boost::shared_ptr<QbertBox> QbertBox_ptr;


	class QbertGameObject : public GameObject
	{
	public:
		float Progress;
		DWORD MoveLength;
		QbertBox * NowOn, * AfterOn;

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

				NowOn = AfterOn;
				AfterOn = model.GetBoxNeibhor(NowOn, direction);

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

	void SetBoxNeibhor(Direction direction, QbertBox& box, QbertBox& newNeibhor)		//Dosen't change None direction.
	{
		switch(direction)
		{
		case(Up) :
			box._neibhors[Up] = &newNeibhor;
		case(Down):
			box._neibhors[Down] = &newNeibhor;
		case(Left):
			box._neibhors[Left] = &newNeibhor;
		case(Right):
			box._neibhors[Right] = &newNeibhor;
		case(None) :
			box._neibhors[None] = &box;
		}
	}

	QbertBox* GetBoxNeibhor (QbertBox& box, Direction direction)
	{
		return box._neibhors[direction];
	}

	QbertBox* GetBoxNeibhor (QbertBox* box, Direction direction)
	{
		return (*box)._neibhors[direction];
	}

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
