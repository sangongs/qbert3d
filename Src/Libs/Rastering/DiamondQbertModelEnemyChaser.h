#pragma once

#include "boost/shared_ptr.hpp"

#include "QbertEnemyObj.h"


namespace BGComplete
{

class DiamondQbertModelEnemyChaser : public QbertEnemyObj
{
private:
	void SetListOfBoxes();

	Direction MoveToPoint(const Math::Point3D& moveTo);

public:
	DiamondQbertModelEnemyChaser(const std::string& name = "", Model* model = NULL, int score = 0, QbertBox_ptr box = QbertBox_ptr(), DWORD moveLegth = 100);
	~DiamondQbertModelEnemyChaser(void) {}

	Direction WhereToMove();
};

typedef boost::shared_ptr<DiamondQbertModelEnemyChaser> DiamondQbertModelEnemyChaser_ptr;

}	//namespace BGComplete
