#include "windows.h"

#include <stdlib.h>

#include <boost/shared_ptr.hpp>

#include "Rastering/SimpleControler.h"
#include "Rastering/SimpleModel.h"
#include "Rastering/SimpleView.h"

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
	{
	SimpleModel model;
	SimpleView view;
	SimpleControler controler((View*)&view, (Model*)&model);

	controler.Run(800, 640, 8);
	
	return 0;
}
