#pragma once

#include "SimpleControler.h"
#include "QbertGameObject.h"

namespace BGComplete
{

class QbertModel; 
class QbertEnemyObj;

typedef Direction (*AIFunction)(boost::shared_ptr<QbertEnemyObj>, QbertModel*);

class QbertEnemyObj : public QbertGameObject
{
	friend class QbertModel;
	
	boost::shared_ptr<QbertEnemyObj> _selfSharedPtr;
private:
	std::string _type;
	AIFunction _AIfunc;

public:
	QbertEnemyObj(const std::string& name ="", QbertBox_ptr box = QbertBox_ptr(), const std::string& type ="", DWORD moveLegth = 100) 
		: QbertGameObject(name, box, moveLegth), _type(type) {_selfSharedPtr = boost::shared_ptr<QbertEnemyObj>(this);}

	Direction WhereToMove();
	bool IsQbertStillAlive();

	~QbertEnemyObj(void);
};
typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;

}	//namespace BGComplete
