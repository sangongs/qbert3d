#pragma once

#include "SimpleControler.h"
#include "QbertGameObject.h"

namespace BGComplete
{
	class QbertModel; 
	typedef Direction (*AIFunction)(QbertModel*);

	class QbertEnemyObj : public QbertGameObject
	{
		friend class QbertModel;

	private:
		std::string _type;
		AIFunction _AIfunc;

	public:
		QbertEnemyObj(const std::string& name ="", QbertBox_ptr box = QbertBox_ptr(), const std::string& type ="", DWORD moveLegth = 100) 
			: QbertGameObject(name, box, moveLegth), _type(type) {};

		Direction WhereToMove();
		bool IsQbertStillAlive();

		~QbertEnemyObj(void);
	};
	typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;
}
