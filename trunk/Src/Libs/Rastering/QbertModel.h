#pragma once

#include <Windows.h>

#include <vector>

#include "DrawableObj.h"
#include "Model.h"

class InputData;

class QbertModel : public Model
{
public:
	enum Direction {Right, Left, Up, Down, None};

	typedef Direction (*AIFunction)(QbertModel&);

	class QbertBox : public GameObject
	{
		friend QbertModel;

		int _face;
		Point3D _upDirection;
		QbertBox* Neibhors[4];
		//QbertBox * Left, * Right, * Up, * Down;
	public:
		Point3D GetUp();
		QbertBox(const std::string& name, const Point3D& upDirection, int face, 
			float x, float y, float z, float xRotate = 0, float yRotate = 0, float zRotate = 0)
			: GameObject(name, x, y, z, xRotate, yRotate, zRotate), _upDirection(upDirection), _face(face) {}
	};

	typedef boost::shared_ptr<QbertBox> QbertBox_ptr;


	class QbertGameObject : public GameObject
	{
	public:
		QbertBox * _nowOn, * _afterOn;

		QbertGameObject(const std::string& name ="", QbertBox* box = NULL)
			: GameObject(name, 0, 0, 0, 0, 0, 0), _nowOn (box), _afterOn(box) {}

		const QbertBox& WhereNow() const;
		const QbertBox& WhereNext() const;

		~QbertGameObject();
	};

	typedef boost::shared_ptr<QbertGameObject> QbertGameObject_ptr;


	class QbertEnemyObj : public QbertGameObject
	{
		std::string _type;
		bool _isFuncIntit;
		AIFunction _AIfunc;
	public:
		QbertEnemyObj(const std::string& name ="", QbertBox* box = NULL, const std::string& type ="") 
			: QbertGameObject(name, box), _isFuncIntit(false), _type(type) {};

		void Move(QbertModel& model)
		{
			if (!_isFuncIntit)
			{
				_AIfunc = model._AITypes.find(_type)->second;
				_isFuncIntit = true;
			}

			Direction direction = _AIfunc(model);

			_nowOn = _afterOn;
			_afterOn = model.GetBoxNeibhor(_nowOn, direction);
		}

		~QbertEnemyObj(void);
	};

	typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;


	class ModelObjects;
	class ModelObjectsIters
	{
	public:
		std::list<QbertGameObject_ptr>::const_iterator QbertStart, QbertEnd;
		std::list<QbertEnemyObj_ptr>::const_iterator EnemiesStart, EnemiesEnd;
		std::list<QbertBox_ptr>::const_iterator BoxesStart, BoxesEnd;

		void Init (const ModelObjects& objects)
		{
			QbertStart = objects._qbertList.begin();
			QbertEnd = objects._qbertList.end();
			EnemiesStart = objects.Enemies.begin();
			EnemiesEnd = objects.Enemies.end();
			BoxesStart = objects.Boxes.begin();
			BoxesEnd = objects.Boxes.end();
		}
	};


	class ModelObjects
	{
		friend class ModelObjectsIters;
	public:
		QbertGameObject_ptr Qbert;
		std::list<QbertEnemyObj_ptr> Enemies;
		std::list<QbertBox_ptr> Boxes;
		std::map<Point3D, QbertBox_ptr> BoxesMap;

		void SetQbertList() 
		{
			_qbertList = std::list<QbertGameObject_ptr>();
			_qbertList.push_back(Qbert);
		}

	private:
		std::list<QbertGameObject_ptr> _qbertList;					//for the Iterator;
	};

protected:
	DWORD _timeForMove;
	DWORD _lastTime;
	bool _isQbertAlive;
	std::string _boxNameAfter, _boxNameBefore;

	float _progress;

	ModelObjects _objects;
	ModelObjectsIters _iters;

	std::map<std::string, AIFunction> _AITypes;

	void SetBoxNeibhor(Direction direction, QbertBox& box, QbertBox& newNeibhor)
	{
		switch(direction)
		{
		case(Up) :
			box.Neibhors[Up] = &newNeibhor;
		case(Down):
			box.Neibhors[Down] = &newNeibhor;
		case(Left):
			box.Neibhors[Left] = &newNeibhor;
		case(Right):
			box.Neibhors[Right] = &newNeibhor;
		}
	}

	QbertBox* GetBoxNeibhor (QbertBox& box, Direction direction)
	{
		return box.Neibhors[direction];
	}

	QbertBox* GetBoxNeibhor (QbertBox* box, Direction direction)
	{
		return (*box).Neibhors[direction];
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
	QbertModel (std::string boxNameBefore, std::string boxNameAfter) : _boxNameBefore(boxNameBefore), _boxNameAfter(_boxNameAfter), _progress(0), _isQbertAlive(true) {}
	virtual QbertModel::ModelObjectsIters GetObjectsToDraw() const = 0;
	virtual void ReadInput(const InputData&) const = 0;		//Deals with time and decides whether to move or not.
	void SetMoveLength (DWORD time) {_timeForMove = time;}
	bool IsQbertAlive() {return _isQbertAlive;}
	virtual void MakeMove() = 0;
};
