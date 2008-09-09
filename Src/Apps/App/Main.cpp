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

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
{
	using namespace BGComplete;
	DiamondQbertModel model(4, "RedBox", "BlueBox", "Qbert", 10.0f);
	model.AddNewEnemyType("ball", "ball", 6000, 500);
	
	DoubleQbertView view(QbertView_Ptr(new ArielView()), QbertView_Ptr(new FPSView()), 0.3f);
	SimpleControler controler((QbertView*)&view, (QbertModel*)&model);

	controler.Run(1000, 1000, 8);
		
	return 0;
}
