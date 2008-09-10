#pragma once

#include "boost/shared_ptr.hpp"

#include "QbertEnemyObj.h"


namespace BGComplete
{

class DiamondQbertModelEnemyBall : public QbertEnemyObj
{
private:
	void SetListOfBoxes();

public:
	DiamondQbertModelEnemyBall(const std::string& name = "", Model* model = NULL, int score = 0, QbertBox_ptr box = QbertBox_ptr(), DWORD moveLegth = 100);

	~DiamondQbertModelEnemyBall(void) {}

	Direction WhereToMove();
};

typedef boost::shared_ptr<DiamondQbertModelEnemyBall> DiamondQbertModelEnemyBall_ptr;

}	//namespace BGComplete
