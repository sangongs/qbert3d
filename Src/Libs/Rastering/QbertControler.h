#pragma once

#include <Windows.h>

#include "SimpleControler.h"

enum Direction {Right, Left, Up, Down, None};

class QbertControler : public SimpleControler
{
public:

	class InputData
	{
	public:
		DWORD CertenTime;
		Direction direction;
	};

	QbertControler(View * view, Model * model) : SimpleControler(view, model) {}
	~QbertControler(void);
};
