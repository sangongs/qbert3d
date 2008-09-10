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


	void Move(QbertGameObject_ptr object, const SimpleControler::InputData& inputData);
	void MoveEnemies(DWORD deltaTime);

	void CreateEnemies (DWORD deltaTime);

	void RemoveEnemy(const QbertEnemyObj_ptr iter);

	void UpdateCenterOfObject(QbertGameObject_ptr object);
	void UpdateFaceAndUpDirections(QbertGameObject_ptr object);
	void ChangeBox(QbertGameObject_ptr object);
	void EndMovement(QbertGameObject_ptr object);

public:
	DiamondQbertModel(int sizeOfDiamond, const std::string& unvisitedBoxName, const std::string& visitedBoxName, const std::string& qbertName,
		int * score, int * livesLeft, int * level, float freeFallAcceleration);
	DiamondQbertModel() {}

	int Size() const {return _size;}
	void ReciveInput(const SimpleControler::InputData&);
	void AddNewEnemyType(const std::string& type, const std::string& name, DWORD firstDelay, DWORD appearanceFrequency, DWORD moveLength, int maxAppearances);

	~DiamondQbertModel();
};

typedef boost::shared_ptr<DiamondQbertModel> DiamondQbertModel_ptr;

}	//namespace BGComplete
