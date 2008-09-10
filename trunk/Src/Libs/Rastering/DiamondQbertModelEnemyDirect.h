#pragma once

#include "boost/shared_ptr.hpp"

#include "QbertEnemyObj.h"

namespace BGComplete
{

class DiamondQbertModelEnemyDirect : public QbertEnemyObj
{
private:
	static const int _maxMovesToChase = 5, _luckContinuation = 3;
	static const float  _luckFactor;
	void SetListOfBoxes();
	int _movesToChase, _movesOfLuckLeft;

	Direction HelperToWhereToMove(bool isXAxis);
public:
	DiamondQbertModelEnemyDirect(const std::string& name = "", Model* model = NULL, int score = 0, QbertBox_ptr box = QbertBox_ptr(), DWORD moveLegth = 100);

	~DiamondQbertModelEnemyDirect(void) {}

	Direction WhereToMove();
};

typedef boost::shared_ptr<DiamondQbertModelEnemyDirect> DiamondQbertModelEnemyDirect_ptr;

}	//namespace BGComplete