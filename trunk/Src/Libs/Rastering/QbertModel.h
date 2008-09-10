#pragma once

#include <Windows.h>

#include <cmath>

#include <vector>
#include <map>

#include "boost/tuple/tuple.hpp"

#include "MathUtils.h"

#include "DrawableObj.h"
#include "Model.h"
#include "SimpleControler.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

enum GameStage {DidntStart, GoingOn, GameOver};
namespace BGComplete
{

class QbertModel : public Model
{
	friend class QbertEnemyObj;

public:
	class ModelObjects
	{
	public:
		int Score, LivesLeft, Level;
		GameStage gameStage;
		QbertGameObject_ptr Qbert;
		std::list<QbertEnemyObj_ptr> Enemies;
		std::list<QbertBox_ptr> Boxes;
		std::list<GameObject_ptr> ObjectsList;
		std::map<Math::Point3D, QbertBox_ptr> BoxMap;
	};
	typedef boost::shared_ptr<ModelObjects> ModelObjects_Ptr;
			
	struct EnemiesAppearanceData
	{
		std::string Name, Type;
		DWORD AppearanceFrequency, TimeSinceLastAppearance, MoveLength, FirstDelay;
		bool IsAppearedOnce;
		int MaxAppearances, TotalAmount, Score;

		EnemiesAppearanceData (const std::string& type, const std::string& name, DWORD firstDelay,
			DWORD appearanceFrequency, DWORD moveLength, int maxAppearances, int score)
				: Name(name), Type(type), FirstDelay(firstDelay), AppearanceFrequency(appearanceFrequency), MoveLength(moveLength),
				TimeSinceLastAppearance(0), MaxAppearances(maxAppearances), TotalAmount(0), Score(score), IsAppearedOnce(false) {}
		EnemiesAppearanceData() {}
		~EnemiesAppearanceData() {}
	};

protected:
	int _boxesUnvisited, *_score, *_livesLeft, *_level;
	float _freeFallAcceleration;
	std::string _visitedBoxName, _unvisitedBoxName;
	QbertBox_ptr _startingBox;
	ModelObjects_Ptr _objects;
	GameStage _gameStage;
	std::list<EnemiesAppearanceData> _enemiesAppearanceData;
	std::list<QbertEnemyObj_ptr> _enemiesToDelete;

	void VisitBox (QbertBox_ptr box);
	void InsertBox (Math::Point3D point, QbertBox_ptr box);
	void SetQbert (std::string qbertName, QbertBox_ptr box);

	void SetEnemysMoveLength (QbertEnemyObj_ptr enemy, DWORD MoveLength) {enemy->_moveLength = MoveLength;}
	void SetEnemysModelToThis (QbertEnemyObj_ptr enemy) {enemy->_model = this;}

	virtual void RemoveEnemy(const QbertEnemyObj_ptr iter) = 0;

	virtual void Move(QbertGameObject_ptr ,const SimpleControler::InputData& inputdata) = 0;
	virtual void MoveEnemies(DWORD deltaTime) = 0;
	virtual void CreateEnemies (DWORD deltaTime) = 0;

public:
	QbertModel (std::string boxNameBefore, std::string boxNameAfter, int * score, int * livesLeft, int* level, float freeFallAcceleration) :
	  _unvisitedBoxName(boxNameBefore), _visitedBoxName(boxNameAfter), _score(score), _livesLeft(livesLeft), _gameStage(DidntStart),
		  _level(level), _freeFallAcceleration(freeFallAcceleration), _boxesUnvisited(0), _objects(new ModelObjects()) {}

	  QbertModel(){}

	QbertModel::ModelObjects_Ptr GetModelObjects();
	void StartGame();

	virtual void AddNewEnemyType(const std::string& type, const std::string& name, DWORD firstDelay, DWORD appearanceFrequency, DWORD moveLength, 
		int maxAppearances, int score) = 0;

	virtual void ReciveInput(const SimpleControler::InputData&) = 0;

	GameStage GetGameStage() {return _gameStage;}
	bool IsGameWon() {return (_boxesUnvisited <= 0);}
	QbertBox_ptr GetBoxAt(const Math::Point3D&) const;
	QbertBox_ptr GetBoxAt(int x, int y, int z) const;
};

typedef boost::shared_ptr<QbertModel> QbertModel_ptr;

}	//namespace BGComplete
