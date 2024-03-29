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
#include "Rastering/ControlKeysView.h"
#include "Rastering/ScoreView.h"
#include "Rastering/GameOverView.h"
#include "Rastering/NewLevelView.h"
#include "Rastering/LifesView.h"
#include "Rastering/GameStartView.h"

#include "Rastering/QbertGameModel.h"

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
{
	using namespace BGComplete;

	QbertGameModel gameModel = QbertGameModel();
	
	std::list<LayeredQbertView::CoordinatedQbertView> listOfQbertViews;
	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr
				(new DoubleQbertView(
					QbertView_Ptr(new FPSView()), 
					QbertView_Ptr(new ArielView(16)), 
					0.3f)),
			LayeredQbertView::QuadCoords(
				Math::Point2D(0.0f, 0.0f), 
				Math::Point2D(1.0f, 1.0f))));

	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr(new ScoreView()),
		LayeredQbertView::QuadCoords(
			Math::Point2D(0.0f, 0.9f), 
			Math::Point2D(0.1f, 0.1f))));

	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr(new GameOverView()),
		LayeredQbertView::QuadCoords(
			Math::Point2D(0.4f, 0.4f), 
			Math::Point2D(0.4f, 0.2f))));

	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr(new GameStartView()),
		LayeredQbertView::QuadCoords(
			Math::Point2D(0.4f, 0.4f), 
			Math::Point2D(0.4f, 0.2f))));


	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr(new NewLevelView()),
		LayeredQbertView::QuadCoords(
			Math::Point2D(0.4f, 0.4f), 
			Math::Point2D(0.2f, 0.2f))));

	listOfQbertViews.push_back(
		LayeredQbertView::CoordinatedQbertView(
			QbertView_Ptr(new LifesView()),
		LayeredQbertView::QuadCoords(
			Math::Point2D(0.001f, 0.85f), 
			Math::Point2D(0.2f, 0.05f))));

	std::list<LayeredQbertView::CoordinatedSimpleView> listOfSimpleViews;
	listOfSimpleViews.push_back(
		LayeredQbertView::CoordinatedSimpleView(
			View_Ptr(new FPSHud("Fonts//CALIBRI.TTF", 24)),
			LayeredQbertView::QuadCoords(Math::Point2D(0.0f, 0.0f), Math::Point2D(0.1f, 0.1f))));

	listOfSimpleViews.push_back(
		LayeredQbertView::CoordinatedSimpleView(
		View_Ptr(new ControlKeysView()),
		LayeredQbertView::QuadCoords(Math::Point2D(0.0f, 0.0f), Math::Point2D(1.0f, 1.0f))));


	LayeredQbertView view(listOfQbertViews, listOfSimpleViews);
	SimpleControler controler((QbertView*)&view, &gameModel);

	controler.Run(1000, 1000, 24);

	return 0;
}
