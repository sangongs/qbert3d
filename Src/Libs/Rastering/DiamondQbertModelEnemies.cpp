#include "stdafx.h"

#include "Point3D.h"

#include "SimpleControler.h"
#include "QbertModel.h"
#include "DiamondQbertModel.h"

namespace BGComplete
{

namespace DiamondQbertModelEnemies
{
	Direction BallAIFunction(QbertEnemyObj_ptr obj, QbertModel* /*model*/)
	{
		srand( (unsigned)time(NULL) );
		if (obj->LastBox->IsOnPerimeter())
			return IntoBox;
		if (obj->MovingDirection == OutOfBox)
			return Direction(rand() % 4);
		if (obj->MovingDirection == Left)
			return ((rand() % 2) ? Right: Up);
		if (obj->MovingDirection == Right)
			return ((rand() % 2) ? Left: Up);

		return Direction(rand() % 3);
	}

	std::vector<AppearanceBox> BallAppearanceBoxes(DiamondQbertModel& model)
	{	
		std::vector<AppearanceBox> ret;
		ret.push_back(model.GetBoxAt(0, model.Size() - 1, 0));
		ret.push_back(model.GetBoxAt(0, - model.Size() + 1, 0));

		return ret;
	}
}	//namespace DiamondQbertModelEnemies

}	//namespace BGComplete