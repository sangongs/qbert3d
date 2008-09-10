#pragma once

#include <vector>

#include "SimpleControler.h"
#include "QbertGameObject.h"

namespace BGComplete
{

class QbertModel; 
class QbertEnemyObj;

class QbertEnemyObj : public QbertGameObject
{
	friend class QbertModel;

protected:

	static std::map<std::string, VecOfAppearanceBox_ptr> _apperanceMap;

	std::string _type;
	int _score;

	virtual void SetListOfBoxes() = 0;

public:
	QbertEnemyObj(const std::string& name = "", Model* model = NULL, int score = 0, QbertBox_ptr box = QbertBox_ptr(), const std::string& type ="", DWORD moveLegth = 100) 
		: QbertGameObject(name, model, box, moveLegth), _type(type), _score(score) {_isQbert = false;}

	virtual Direction WhereToMove() = 0;
	bool IsQbertDies();

	const std::string& GetType() const {return _type;}
	std::string GetType() {return _type;}
	int GetScore() {return _score;}
	VecOfAppearanceBox_ptr GetAppearanceBoxes();

	~QbertEnemyObj(void);

};
typedef boost::shared_ptr<QbertEnemyObj> QbertEnemyObj_ptr;

}	//namespace BGComplete
