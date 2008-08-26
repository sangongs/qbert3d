#pragma once

#include <Windows.h>

#include "SimpleControler.h"
#include "QbertModel.h"


class QbertControler : public SimpleControler
{
public:
	class InputData
	{
	public:
		DWORD CertenTime;
		QbertModel::Direction direction;
	};

	QbertControler(View * view, Model * model) : SimpleControler(view, model) {}
	~QbertControler(void);
};
