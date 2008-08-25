#pragma once

#include "QbertModel.h"

class DiamondQbertModel : public QbertModel 
{
public:
	DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName);

	QbertModel::ModelObjectsIters GetObjectsToDraw() const;
	void ReadInput(const QbertControler::InputData&);			//Deals with time and decides whether to move or not.
	void MakeEnemiesMove();

	~DiamondQbertModel();
};
