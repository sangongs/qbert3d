#pragma once

namespace BGComplete
{
	class Point3D;

	class DrawableObj
	{
	private:
		int _listNum;
		float _yDistFromFloor;

	public:
		DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, float scale, float rotateX, float rotateY, float rotateZ);
		~DrawableObj();

		void Draw(float rotateX, float rotateY, float rotateZ, float scale);
	};

	typedef boost::shared_ptr<DrawableObj> DrawableObj_Ptr;
}
