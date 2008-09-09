#pragma once

#include "QbertModel.h"
#include "SimpleControler.h"

namespace BGComplete
{

class DiamondQbertModel : public QbertModel 
{
private:
	int _size;
	void CreateDiamondStructure(int sizeOfDiamond);
	void SetupQbert(int sizeOfDiamond, const std::string& qbertName);

public:
	DiamondQbertModel(int sizeOfDiamond, const std::string& boxNameBefore, const std::string& boxNameAfter, const std::string& qbertName
		, float freeFallAcceleration);


	int Size() const {return _size;}

	void ReciveInput(const SimpleControler::InputData&);
	void Move(QbertGameObject_ptr object, const SimpleControler::InputData& inputData);
	void MoveEnemies(DWORD deltaTime);

	void AddNewEnemyType(const std::string& type, const std::string& name, DWORD appearanceFrequency, DWORD moveLength);
	void CreateEnemies (DWORD deltaTime);

	void UpdateCenterOfObject(QbertGameObject_ptr object);
	void UpdateFaceAndUpDirections(QbertGameObject_ptr object);
	void ChangeBox(QbertGameObject_ptr object);
	void EndMovement(QbertGameObject_ptr object);

	~DiamondQbertModel();
};

}	//namespace BGComplete
