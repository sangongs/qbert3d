#include "windows.h"

#include <stdlib.h>

#include <boost/shared_ptr.hpp>

#include "Rastering/Point3D.h"
#include "Rastering/SimpleControler.h"
#include "Rastering/DiamondQbertModel.h"
#include "Rastering/SimpleView.h"

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
	{
	BGComplete::DiamondQbertModel model(2, "RedBox", "BlueBox", "Qbert", 10.0f);
	BGComplete::SimpleView view;
	BGComplete::SimpleControler controler((BGComplete::View*)&view, (BGComplete::QbertModel*)&model);

	controler.Run(1000, 1000, 8);
		
	return 0;
}
