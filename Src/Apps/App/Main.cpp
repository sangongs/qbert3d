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
	int livesLeft = 3, score = 0;
	DiamondQbertModel model(5, "RedBox", "BlueBox", "Qbert", &score, &livesLeft, 10.0f);
	model.AddNewEnemyType("ball", "ball", 1000, 1000, 500, 5);
	//model.AddNewEnemyType("directEnemy", "directEnemy", 2500, 1500, 1200, 6);
	
	std::list<LayeredQbertView::CoordinatedQbertView> listOfQbertViews;
	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr
				(new DoubleQbertView(
					QbertView_Ptr(new ArielView(16)), 
					QbertView_Ptr(new FPSView()), 
					0.3f)),
			LayeredQbertView::QuadCoords(
				Math::Point2D(0, 0), 
				Math::Point2D(1, 1))));

	std::list<LayeredQbertView::CoordinatedSimpleView> listOfSimpleViews;
	float fpsHudPoints[] = 
		{-2.0f, 2.0f, -5.0f,
		2.0f, 2.0f, -5.0f,
		2.0f, -2.0f, -5.0f,
		-2.0f, -2.0f, -5.0f};
	listOfSimpleViews.push_back(
		LayeredQbertView::CoordinatedSimpleView(
			View_Ptr(new FPSHud("Fonts//CALIBRI.TTF", 24, fpsHudPoints)),
			LayeredQbertView::QuadCoords(Math::Point2D(0.0f, 0.0f), Math::Point2D(0.1f, 0.1f))));

	LayeredQbertView view(listOfQbertViews, listOfSimpleViews);
	SimpleControler controler((QbertView*)&view, (QbertModel*)&model);

	controler.Run(1000, 1000, 24);

	return 0;
}
