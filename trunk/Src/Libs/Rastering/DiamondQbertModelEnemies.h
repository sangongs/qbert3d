#include "GameObject.h"

namespace BGComplete
{

class DiamondQbertModel;

namespace DiamondQbertModelEnemies
{

	Direction BallAIFunction(QbertEnemyObj_ptr, QbertModel*);
	std::vector<AppearanceBox> BallAppearanceBoxes(const DiamondQbertModel&);

}	//namespace DiamondQbertModelEnemies

}	//namespace BGComplete