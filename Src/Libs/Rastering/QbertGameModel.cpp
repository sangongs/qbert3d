#include "StdAfx.h"

#include "Model.h"
#include "QbertModel.h"
#include "DiamondQbertModel.h"

#include "QbertGameModel.h"

namespace BGComplete
{

const std::string QbertGameModel::_qbertName = "Qbert";

QbertGameModel::QbertGameModel() : _qbertLivesLeft(_defaultLivesNum), _currentScore(0), _currentLevel(0), _gameStage(DidntStart)
{
	_returnObject = QbertModel::ModelObjects_Ptr(new QbertModel::ModelObjects());
	_boxNames.push_back("RedBox");
	_boxNames.push_back("BlueBox");
}

QbertGameModel::~QbertGameModel(void) {}

QbertModel::ModelObjects_Ptr QbertGameModel::GetModelObjects()
{
	if (_gameStage == GoingOn)
		return _currentQbertModel->GetModelObjects();

	_returnObject->gameStage = _gameStage;
	return _returnObject;
}

void QbertGameModel::ReciveInput(const SimpleControler::InputData& data)
{
	if (data.StartNewGame)
	{
		SetupNewGame();
		return;
	}

	if (_gameStage == GoingOn)
	{
		_currentQbertModel->ReciveInput(data);
		_gameStage = _currentQbertModel->GetGameStage();

		if(_currentQbertModel->IsGameWon())
			SetupNextLevel();
		return;
	}
}

void QbertGameModel::SetupNewGame()
{
	_currentScore = 0;
	_currentLevel = 5;
	_qbertLivesLeft = _defaultLivesNum;
	SetupNextLevel();
	_gameStage = GoingOn;
}

void QbertGameModel::SetupNextLevel()
{
	_currentLevel++;

	boost::mt19937 generator((boost::uint32_t)std::time(0));
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> uniRand(generator, boost::uniform_int<>(0, (int)_boxNames.size() - 1));
	
	std::string firstBoxName = _boxNames[uniRand()], secondBoxName = _boxNames[uniRand()];

	while (firstBoxName == secondBoxName)														//make sure the boxes are different
		secondBoxName = _boxNames[uniRand()];

	_currentQbertModel = DiamondQbertModel_ptr(new DiamondQbertModel((_currentLevel + 3) / 2, firstBoxName, secondBoxName, _qbertName, &_currentScore, &_qbertLivesLeft, 10.0f));
	_currentQbertModel->AddNewEnemyType("ball", "ball", 2500, 550, 600, 2);					//[todo] implement some function for those to make levels harder but playable
	_currentQbertModel->AddNewEnemyType("directEnemy", "directEnemy", 2700, 500, 500, 5);		//[todo] implement some function for those to make levels harder but playable
	_currentQbertModel->StartGame();
	
	
	Sleep(1500);		//delay between levels!
}


}	//namespace BGComplete
