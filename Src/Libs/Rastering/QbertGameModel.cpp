#include "StdAfx.h"

#include "Model.h"
#include "QbertModel.h"
#include "DiamondQbertModel.h"

#include "QbertGameModel.h"

namespace BGComplete
{

const std::string QbertGameModel::_qbertName = "Qbert";

QbertGameModel::QbertGameModel() : _qbertLivesLeft(_defaultLivesNum), _currentScore(0), _currentLevel(0), _nextLifeAt(0),
	_gameStage(DidntStart), _isGameInitialized(false)
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
	_returnObject->LivesLeft = _qbertLivesLeft;
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
		if (!_isGameInitialized)
		{
			_isGameInitialized = true;
			return;
		}

		_currentQbertModel->ReciveInput(data);
		_gameStage = _currentQbertModel->GetGameStage();

		if(_currentQbertModel->IsGameWon())
		{
			if (_currentLevel < _lastLevel)
				SetupNextLevel();
			else
				_gameStage = GameWon;
		}
	}
}

void QbertGameModel::SetupNewGame()
{
	_currentScore = 0;
	_currentLevel = 0;
	_nextLifeAt = 700;
	 _addToNextLifeAt = 900;
	_qbertLivesLeft = _defaultLivesNum;
	SetupNextLevel();
	_gameStage = GoingOn;
}

void QbertGameModel::SetupNextLevel()
{
	_currentLevel++;

	int numOfChasers = 0;

	if (_currentLevel == _lastLevel)
		numOfChasers = 2;
	else if (_currentLevel > 0.75 * _lastLevel - 1)
		numOfChasers = 1;

	_currentQbertModel = QbertModel_ptr(new DiamondQbertModel(_currentLevel + 2, 
		_boxNames[_currentLevel % _boxNames.size()].first, _boxNames[_currentLevel % _boxNames.size()].second,
		_qbertName, &_currentScore, &_qbertLivesLeft, &_currentLevel, &_nextLifeAt, & _addToNextLifeAt, 10.0f));
	_currentQbertModel->AddNewEnemyType("ball", "ball", 3000, 2800, 1000, 3 * _currentLevel, _currentLevel > _lastLevel / 2 ? 10 : 0);
	_currentQbertModel->AddNewEnemyType("directEnemy", "directEnemy", 3000, 3250, 1350, _currentLevel, 35);
	_currentQbertModel->AddNewEnemyType("chaser", "chaser", 1500, 2500, (numOfChasers == 1) ? 1200 : 1400, numOfChasers, 300);
	_currentQbertModel->StartGame();
	

	_isGameInitialized = false;		//in order to skip the first input witchs time is wrong due to the initalizing of the level

	Sleep(1500);					//delay between levels!
}


}	//namespace BGComplete
