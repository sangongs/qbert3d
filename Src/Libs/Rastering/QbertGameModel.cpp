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
	_boxNames.push_back(std::pair<std::string, std::string>("RedBox", "BlueBox"));
	_boxNames.push_back(std::pair<std::string, std::string>("BlueBox", "RedBox"));
}

QbertGameModel::~QbertGameModel(void) {}

QbertModel::ModelObjects_Ptr QbertGameModel::GetModelObjects()
{
	if (_gameStage == GoingOn)
		return _currentQbertModel->GetModelObjects();

	_returnObject->Score = _currentScore;
	_returnObject->Level = _currentLevel;
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
	_currentLevel = 0;
	_qbertLivesLeft = _defaultLivesNum;
	SetupNextLevel();
	_gameStage = GoingOn;
}

void QbertGameModel::SetupNextLevel()
{
	_currentLevel++;

	_currentQbertModel = QbertModel_ptr(new DiamondQbertModel((_currentLevel + 3) / 2, 
		_boxNames[_currentLevel % _boxNames.size()].first, _boxNames[_currentLevel % _boxNames.size()].second,
		_qbertName, &_currentScore, &_qbertLivesLeft, &_currentLevel, 10.0f));
	_currentQbertModel->AddNewEnemyType("ball", "ball", 2500, 1200, 1200, 3, 10);					//[todo] implement some function for those to make levels harder but playable
	//_currentQbertModel->AddNewEnemyType("directEnemy", "directEnemy", 2700, 1500, 1500, 10);		//[todo] implement some function for those to make levels harder but playable
	_currentQbertModel->StartGame();
	
	
	Sleep(1500);		//delay between levels!
}


}	//namespace BGComplete
