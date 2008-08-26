#pragma once

#include "QbertModel.h"
#include "QbertControler.h"

class DiamondQbertModel : public QbertModel 
{
public:
	DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName);

	QbertModel::ModelObjectsIters GetObjectsToDraw() const;
	void ReadInput(const QbertControler::InputData&);			//Deals with time and decides whether to move or not.
	void QbertMove(const QbertControler::InputData& inputdata, DWORD deltaTime);
	void MakeEnemiesMove(DWORD deltaTime);

	~DiamondQbertModel();
};
