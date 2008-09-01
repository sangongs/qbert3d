#pragma once

#include "QbertModel.h"
#include "SimpleControler.h"

class DiamondQbertModel : public QbertModel 
{
public:
	DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName
		, float freeFallAcceleration);

	std::list<GameObject_ptr>* GetObjects();
	void ReciveInput(const SimpleControler::InputData&);
	void Move(QbertGameObject_ptr object, const SimpleControler::InputData& inputData);
	void MakeEnemiesMove(DWORD deltaTime);

	~DiamondQbertModel();
};
