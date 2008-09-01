#pragma once

#include "View.h"
#include "Model.h"

enum Direction {Right, Left, Up, Down, None};

class QbertModel;

class SimpleControler
{
private:
	View * _view;
	QbertModel * _model;
	bool _leftMouseKeyDown, _running;

	void ReadInput(DWORD deltaTime);

public:
	SimpleControler(View * view, QbertModel * model);
	virtual void Run(unsigned startWidth, unsigned startHeight, unsigned depth);
	
	class InputData
	{
	public:
		DWORD DeltaTime;
		Direction direction;

		InputData (DWORD deltaTime = 0, Direction direc = None) : DeltaTime(deltaTime), direction(direc) {}
	};
};
