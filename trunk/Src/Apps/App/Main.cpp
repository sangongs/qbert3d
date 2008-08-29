#include "windows.h"

#include <stdlib.h>

#include <boost/shared_ptr.hpp>

#include "Rastering/SimpleControler.h"
#include "Rastering/DiamondQbertModel.h"
#include "Rastering/SimpleView.h"

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
	{
	DiamondQbertModel model(4, "RedBox", "BlueBox", "Qbert");
	SimpleView view;
	SimpleControler controler((View*)&view, (QbertModel*)&model);

	controler.Run(1000, 1000, 8);
	
	return 0;
}
