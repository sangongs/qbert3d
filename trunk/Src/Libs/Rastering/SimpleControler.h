#pragma once

#include "View.h"
#include "Model.h"
/*#include "QbertModel.h"*/

enum Direction {Right, Left, Up, Down, None};

class QbertModel;

class SimpleControler
{
private:

	View * _view;
	QbertModel * _model;

	bool _leftMouseKeyDown, _running;

	void Input(DWORD deltaTime);

public:
	class InputData
	{
	public:
		DWORD DeltaTime;
		Direction direction;
	};


	SimpleControler(View * view, QbertModel * model);
	
	virtual void Run(unsigned startWidth, unsigned startHeight, unsigned depth);
};
