#pragma once

namespace BGComplete
{
	enum Direction {Right, Left, Up, Down, None, OutOfBox, IntoBox};

	class QbertModel;
	class QbertView;

	class SimpleControler
	{
	private:
		QbertView * _view;
		QbertModel * _model;
		bool _leftMouseKeyDown, _running;

		void ReadInput(DWORD deltaTime);

	public:
		SimpleControler(QbertView * view, QbertModel * model);
		virtual void Run(unsigned startWidth, unsigned startHeight, unsigned depth);
		
		class InputData
		{
		public:
			DWORD DeltaTime;
			Direction direction;

			InputData (DWORD deltaTime = 0, Direction direc = None) : DeltaTime(deltaTime), direction(direc) {}
		};
	};
}
