#pragma once

#include "boost\shared_ptr.hpp"

#include "QbertModel.h"

namespace BGComplete
{

class QbertGameModel : public Model
{
	static const int _defaultLivesNum = 4, _lastLevel = 5;
	static const std::string _qbertName;

	GameStage _gameStage;
	QbertModel_ptr _currentQbertModel;
	QbertModel::ModelObjects_Ptr _returnObject;
	int _qbertLivesLeft, _currentScore, _currentLevel, _nextLifeAt, _addToNextLifeAt;
	std::vector<std::pair<std::string, std::string>> _boxNames;
	bool _isGameInitialized;
public:
	QbertGameModel(void);
	~QbertGameModel(void);

	boost::shared_ptr<QbertModel::ModelObjects> GetModelObjects();
	void ReciveInput(const SimpleControler::InputData&);

	void SetupNewGame();
	void SetupNextLevel();
	
};

}	//namespace BGComplete
