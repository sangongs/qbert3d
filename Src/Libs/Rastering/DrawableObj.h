#pragma once

class DrawableObj
{
private:
	int _listNum;

public:
	DrawableObj(const std::string &directory, const std::string &fileName, const std::string &objName, float scale);
	~DrawableObj();
	
	void Draw();
};

typedef boost::shared_ptr<DrawableObj> DrawableObj_Ptr;
