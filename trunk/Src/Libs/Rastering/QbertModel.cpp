#include "stdafx.h"

#include "Point3D.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

#include "QbertModel.h"

namespace BGComplete
{

void QbertModel::VisitBox (QbertBox_ptr box)
{
	if (box->_isVisited)
		return;

	box->_isVisited = true;
	box->Name = _visitedBoxName;
	*_score += 25;
	_boxesUnvisited--;
}

void QbertModel::InsertBox (Math::Point3D point, QbertBox_ptr box)
{
	_objects->BoxMap.insert(std::pair<Math::Point3D, QbertBox_ptr>(point, box));
	_objects->Boxes.push_back(box);
}

void QbertModel::SetQbert (std::string qbertName, QbertBox_ptr box)
{
	_objects->Qbert = QbertGameObject_ptr(new QbertGameObject(qbertName, this, box));
}

QbertBox_ptr QbertModel::GetBoxAt(const Math::Point3D& point) const
{
	std::map<Math::Point3D, QbertBox_ptr>::const_iterator box = _objects->BoxMap.find(point);
	return (box != _objects->BoxMap.end()) ? box->second : QbertBox_ptr();
}

QbertBox_ptr QbertModel::GetBoxAt(int x, int y, int z) const
{
	std::map<Math::Point3D, QbertBox_ptr>::const_iterator box = _objects->BoxMap.find(Math::Point3D((float)x, (float)y, (float)z));
	return (box != _objects->BoxMap.end()) ? box->second : QbertBox_ptr();
}

QbertModel::ModelObjects_Ptr QbertModel::GetModelObjects()
{
	_objects->Score = *_score; 
	_objects->LivesLeft = *_livesLeft;
	_objects->Level = *_level;
	_objects->gameStage = _gameStage;
	return _objects;
}

void QbertModel::StartGame()
{
	_gameStage = GoingOn;
	ReciveInput(SimpleControler::InputData());
}

void QbertModel::GetAdditionalLife()
{
	if (*_score > *_gainLifeAt)
	{
		(*_livesLeft)++;
		*_gainLifeAt += *_addToNextLifeAt;
		*_addToNextLifeAt += 200;
	}
}

}	//namespace BGComplete
