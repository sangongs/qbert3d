#pragma once


namespace BGComplete
{
	class QbertGameModel;

	enum Direction {Right, Left, Up, Down, None, OutOfBox, IntoBox};

	class QbertModel;
	class QbertView;

	class SimpleControler
	{
	private:
		QbertView * _view;
		QbertGameModel * _model;
		bool _leftMouseKeyDown, _running;
		unsigned _width, _height;

		void ReadInput(DWORD deltaTime);

	public:
		SimpleControler(QbertView * view, QbertGameModel * model);
		virtual void Run(unsigned startWidth, unsigned startHeight, unsigned depth);
		
		class InputData
		{
		public:
			DWORD DeltaTime;
			Direction direction;
			bool StartNewGame;

			InputData (DWORD deltaTime = 0, Direction direc = None, bool startNewGame = false) 
				: DeltaTime(deltaTime), direction(direc), StartNewGame(startNewGame) {}
		};
	};
}
