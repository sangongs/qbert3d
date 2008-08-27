#pragma once

#include "QbertModel.h"
#include "SimpleControler.h"

class DiamondQbertModel : public QbertModel 
{
public:
	DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName);

	std::list<GameObject_ptr>* GetObjects();
	void ReadInput(const SimpleControler::InputData&);			//Deals with time and decides whether to move or not.
	void QbertMove(const SimpleControler::InputData& inputdata);
	void MakeEnemiesMove(DWORD deltaTime);

	~DiamondQbertModel();
};
