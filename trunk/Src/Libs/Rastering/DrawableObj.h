#pragma once

class DrawableObj
{
private:
	int _listNum;
	float _yMin;

public:
	DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, const std::string &objName, float scale, float rotateX, float rotateY, float rotateZ);
	~DrawableObj();
	
	void Draw(float rotateX, float rotateY, float rotateZ, float scale);
};

typedef boost::shared_ptr<DrawableObj> DrawableObj_Ptr;
