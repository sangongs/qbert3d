#pragma once

class SimpleControler
{
private:

	View * _view;
	Model * _model;

	bool _leftMouseKeyDown, _running;

	void Input();

public:
	SimpleControler(View * view, Model * model);
	
	void Run();
};
