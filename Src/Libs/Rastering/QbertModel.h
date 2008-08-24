#pragma once

#include <Windows.h>

#include <vector>

#include "DrawableObj.h"
#include "Model.h"

class InputData;

class QbertModel : public Model
{
public:
	enum Direction {Right, Left, Up, Down};


	class ModelCoordinates
	{
		int _face;				//on witch face the object is;
		int level;				//how high the object is on the face;
		int position;			//how right on the face the object is;

	};


	class QbertBox : public GameObject
	{
		Point3D _up;
		QbertBox *_left, *_right, *_up_, *_down;
	public:
		Point3D GetUp();
	};


	class QbertGameObject : public GameObject
	{
		ModelCoordinates _modelCoordinate;
		QbertBox * _nowOn, * _afterOn;
	public:
		QbertGameObject();

		ModelCoordinates WhereNow() const;
		ModelCoordinates WhereNext() const;

		~QbertGameObject();
	};


	class QbertEnemyObj : public QbertGameObject
	{
		std::string _type;
	public:
		QbertEnemyObj();

		void DecideOnMove(const QbertModel&);
		~QbertEnemyObj(void);
	};

	struct ModelObjects
	{
		QbertGameObject Qbert;
		std::vector<QbertEnemyObj> Enemies;
		std::vector<QbertBox> Boxes;
	};
private:
	DWORD _timeForMove;
	DWORD _lastTime;

	float _progress;

	ModelObjects _objects;

	std::map<std::string, Direction* (QbertModel&)> _AITypes;
public:
	virtual ModelObjects GetObjectsToDraw() const = 0;
	virtual Point3D QbertInfo() const = 0;
	virtual void ReadInput(const InputData&) const = 0;		//Deals with time and decides whether to move or not.
	void SetMoveLength (DWORD time) {_timeForMove = time;}
	virtual void MakeMove() = 0;
};
