#include "windows.h"

#include <stdlib.h>

#include <boost/shared_ptr.hpp>

#include "Rastering/Point3D.h"
#include "Rastering/SimpleControler.h"
#include "Rastering/DiamondQbertModel.h"
#include "Rastering/QbertView.h"
#include "Rastering/ArielView.h"
#include "Rastering/FPSView.h"
#include "Rastering/DoubleQbertView.h"
#include "Rastering/LayeredQbertView.h"
#include "Rastering/FontView.h"
#include "Rastering/FPSHud.h"


int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
{
	using namespace BGComplete;
	DiamondQbertModel model(5, "RedBox", "BlueBox", "Qbert", 10.0f);
	//model.AddNewEnemyType("ball", "ball", 1000, 500);
	model.AddNewEnemyType("directEnemy", "directEnemy", 1500, 1200);
	
	std::list<QbertView_Ptr> listOfQbertViews;
	listOfQbertViews.push_back(
		QbertView_Ptr
			(new DoubleQbertView(
				QbertView_Ptr(new ArielView()), 
				QbertView_Ptr(new FPSView()), 
				0.3f)));

	std::list<View_Ptr> listOfSimpleViews;
	float fpsHudPoints[] = 
		{-5.0f, -3.0f, -10.0f,
		-4.0f, -3.0f, -10.0f,
		-4.0f, -4.0f, -10.0f,
		-5.0f, -4.0f, -10.0f};
	listOfSimpleViews.push_back(View_Ptr(new FPSHud("Fonts//CALIBRI.TTF", 30, fpsHudPoints)));

	LayeredQbertView view(listOfQbertViews, listOfSimpleViews);
	SimpleControler controler((QbertView*)&view, (QbertModel*)&model);

	controler.Run(1000, 1000, 24);

	return 0;
}
