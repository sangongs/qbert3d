#pragma once

#include "boost/shared_ptr.hpp"

#include "QbertEnemyObj.h"

namespace BGComplete
{

class DiamondQbertModelEnemyDirect : public QbertEnemyObj
{
private:
	void SetListOfBoxes();
	int _movesToChase;

	Direction HelperToWhereToMove(bool isXAxis);
public:
	DiamondQbertModelEnemyDirect(const std::string& name = "", Model* model = NULL, QbertBox_ptr box = QbertBox_ptr(), DWORD moveLegth = 100);

	~DiamondQbertModelEnemyDirect(void) {}

	Direction WhereToMove();
};

typedef boost::shared_ptr<DiamondQbertModelEnemyDirect> DiamondQbertModelEnemyDirect_ptr;

}	//namespace BGComplete