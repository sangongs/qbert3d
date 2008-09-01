#include "stdafx.h"

#include "QbertModel.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"


void QbertModel::VisitBox (QbertBox_ptr box)
{
	if (box->_isVisited)
		return;

	box->_isVisited = true;
	box->Name = _visitedBoxName;
	_boxesUnvisited--;
}

void QbertModel::InsertBox (Point3D point, QbertBox_ptr box)
{
	_objects.BoxMap.insert(std::pair<Point3D, QbertBox_ptr>(point, box));
	_objects.Boxes.push_back(box);
}

void QbertModel::SetQbert (std::string qbertName, QbertBox_ptr box)
{
	_objects.Qbert = QbertGameObject_ptr(new QbertGameObject(qbertName, box));
}