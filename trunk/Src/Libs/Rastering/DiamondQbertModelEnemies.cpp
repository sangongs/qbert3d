#include "stdafx.h"

#include "Point3D.h"

#include "SimpleControler.h"
#include "QbertModel.h"
#include "DiamondQbertModel.h"

namespace BGComplete
{

namespace DiamondQbertModelEnemies
{
	Direction BallAIFunction(QbertModel* /*model*/)
	{
		srand( (unsigned)time(NULL) );
		if (LastBox->IsOnPerimeter())
			return IntoBox;
		if (MovingDirection == OutOfBox)
			return Direction(rand() % 4);
		if (MovingDirection == Left)
			return ((rand() % 2) ? Right: Up);
		if (MovingDirection == Right)
			return ((rand() % 2) ? Left: Up);

		return Direction(rand() % 3);
	}

	std::vector<AppearanceBox> BallAppearanceBoxes(const DiamondQbertModel& model)
	{	
		std::vector<AppearanceBox> ret;
		ret.push_back(AppearanceBox(model.GetBoxAt(0, model.Size() - 1, 0), Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
		ret.push_back(AppearanceBox(model.GetBoxAt(0, - model.Size() + 1, 0), Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));

		return ret;
	}

}	//namespace DiamondQbertModelEnemies

}	//namespace BGComplete