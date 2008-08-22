#pragma once

class DrawableObj
{
private:
	int _listNum;

public:
	DrawableObj::DrawableObj(const std::string &fileName, const std::string &objName);

	void Draw();
};
