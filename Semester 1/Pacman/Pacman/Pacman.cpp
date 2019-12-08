#include "Pacman.h"

#include <sstream>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv),_cPacmanSpeed(0.1f),_cPacmanFrameTime(250),_cMunchieFrameTime(500),cSpawnDistance(64),cTileNum(768),cTileSize(32){
	//Initialise Member Variables
	_ghost = new Enemy[ghostCount]();
	_cherry = new Food();
	srand(time(NULL));
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Food();
		_munchies[i]->_CurrentFrameTime = 0;
		_munchies[i]->_FrameCount = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}
	_editor = new LevelEditor();
	_pacman = new Player();
	_editorColl = new PlayerColl();
	_pacmanColl = new PlayerColl();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	//Initialise Remaining Variables
	_paused = false;
	_pKeyDown = false;
	_startGame = false;
	_spaceKeyDown = false;
	
	_pacman->_Direction = 0;
	_pacman->_CurrentFrameTime = 0;
	_pacman->_Frame = 0;
	_pacman->speedMulti = 1.0f;
	_pacman->sprint = false;
	_pacman->sprintAnim = false;
	_pacman->super = false;
	_pacman->superTimer = 0;
	_pacman->dead = false;
	_pacman->immune = false;
	_pacman->health = 100;
	debug = false;
	editor = false;
	editInitial = false;
	tile = 'a';

	for (int i = 0; i < cTileNum; i++)
	{
		file[i] = 'Z';
	}

	_ghost->direction = 0;
	_ghost->speed = 0.2f;

	ghostCount = 0;
	levelSave = false;
	level = 1;

	pacmanProj = false;
	roomClear = false;
	score = 0;
	highScore = 0;
	scoreMulti = 1.0f;

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//Clean Up Pointers In Structure
	delete _pacman->_Texture;
	delete _pacman->_SourceRect;
	delete _pacman->_Position;
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->_Rect;
	}
	delete _munchies[0]->_Texture;

	delete _cherry->_position;
	delete _cherry ->_Texture;

	//Clean Up Pacman Structure Pointer
	delete _pacman;
	delete[] _munchies;
	delete _cherry;

	delete _menuBackground;
	delete _menuRectangle;
}

void Pacman::LoadContent()
{
	Pacman::CreateLevel();

	// Load Pacman
	_pacman->_Texture = new Texture2D();
	_pacman->_Texture->Load("Textures/Pacman.tga", false);
	_pacman->_SourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacmanColl->rect = new Rect(0.0f, 0.0f, 1, 1);
	_pacmanColl->texture = new Texture2D();
	_pacmanColl->texture->Load("texture", false);

	_editorColl->rect = new Rect(0.0f, 0.0f, 1, 1);
	_editorColl->position = new Vector2(0.0f, 0.0f);
	
	_cherry->_Texture = new Texture2D();
	_cherry->_Texture->Load("Textures/Food/Cherry.png", true);
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set Menu Parameters
	_menuPosition = new Vector2(0.0f,0.0f);
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Menu/Transparency.png", false);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard and mouse
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	Pacman::CheckEditor(keyboardState, Input::Keys::F10);
	//Start Game
	Pacman::CheckStart(keyboardState, Input::Keys::SPACE);
	//Checks If The Game Has Started 
	//_startGame = true;
	if (_startGame)
	{
		//Checks If Game Is Paused Before Updating Anything Else
		Pacman::CheckPaused(keyboardState, Input::Keys::P);
		if (_pacman->dead)
		{
			Pacman::Restart(keyboardState, Input::Keys::SPACE);
		}

		if (!_paused) 
		{
			Pacman::Input(elapsedTime, keyboardState, mouseState);

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				Pacman::UpdateMunchie(_munchies[i], elapsedTime);
			}
			Pacman::UpdatePacman(elapsedTime);

			Pacman::UpdateGhost(_ghost, elapsedTime);
			
			if (_pacman->sprint == true)
			{
				Pacman::Sprint();
			}
			if (_pacman->super == true)
			{
				Pacman::Super();
			}
			if (_pacman->immune == true)
			{
				Pacman::Immune();
			}
			if (_pacman->dead)
			{
				_paused = true;
			}
			else
			{
				_paused = false;
			}
			if (editor == true)
			{
				Pacman::Editor();
			}

			//CollisionChecks
			Pacman::CheckCollision();

			//Confine Pacman To Screen
			Pacman::CheckViewportCollision();
			if (score > highScore)
			{
				highScore = score;
			}
		}
	}
	
	
}

void Pacman::Draw(int elapsedTime)
{
	//Rectangle For Menus
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	std::stringstream stream;
	SpriteBatch::BeginDraw(); // Starts Drawing
	for (int i = 0; i < cTileNum; i++)
	{
		SpriteBatch::Draw(_tile[i]->texture, _tile[i]->position, _tile[i]->rect);
	}
	if (editor == false)
	{
		// Allows us to easily create a string
		
		stream << "Health: " << _pacman->health << "     Score: " << score << "     Level: " << level << "     High Score: " << highScore;

		//Draw Munchie
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchies[i]->_Texture, _munchies[i]->_position, _munchies[i]->_Rect);
		}

		//Draw Cherry
		SpriteBatch::Draw(_cherry->_Texture, _cherry->_position, _cherry->_Rect);

		//Draw Enemies
		SpriteBatch::Draw(_ghost->texture, _ghost->position, _ghost->sourceRect);

		//Draw Pacman
		if (!_pacman->dead)
		{
			SpriteBatch::Draw(_pacman->_Texture, _pacman->_Position, _pacman->_SourceRect);
		}
	}


	//SpriteBatch::Draw(_pacmanColl->texture, _pacmanColl->position, _pacmanColl->rect);

	// Draws Pause Menu
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (editor == false)
	{
		if (_paused)
		{
			if (_pacman->dead)
			{
				std::stringstream menuStream;
				menuStream << "YOU DIED \n Press Space To Restart";
				SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
				SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
			}
			else
			{
				std::stringstream menuStream;
				menuStream << "PAUSED!";

				SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
				SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
			}
		}

		// Draws Start Game
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		if (!_startGame)
		{
			std::stringstream menuStream;
			menuStream << "PRESS SPACE TO START THE GAME";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
	}
	

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::CheckCollision()
{
	//Munchie Collision
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
			_munchies[i]->_position->X, _munchies[i]->_position->Y, _munchies[i]->_Rect->Width, _munchies[i]->_Rect->Height))
		{
			_munchies[i]->_position = new Vector2(-100, -100);
			score += (100 * scoreMulti);
		}
	}
	//Cherry Collision
	if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
		_cherry->_position->X, _cherry->_position->Y, _cherry->_Rect->Width, _cherry->_Rect->Height))
	{
		_cherry->_position = new Vector2(-100, -100);
		score += (1000 * scoreMulti);
		_pacman->super = true;
	}
	if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
		_ghost->position->X, _ghost->position->Y, _ghost->sourceRect->Width, _ghost->sourceRect->Height))
	{
		if(_pacman->immune)
		{
		}
		else if (_pacman->super == true || _pacman->sprint == true)
		{
			_ghost->position = new Vector2(-100, -100);
			score += (2500 * scoreMulti);
		}
		else if (!_pacman->immune)
		{
			_pacman->health -= 10;
			_pacman->immune = true;
		}
	}
	if (editor==true)
	{
		for (int i = 0; i < cTileNum; i++)
		{
			if (TileCollisionCheck(_editorColl->position->X, _editorColl->position->Y, 1, 1,
				_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
			{
				if (tile == 'A')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BendDownleft.png", false);
					file[i] = 'A';
				}
				if (tile == 'B')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BendDownRight.png", false);
					file[i] = 'B';
				}
				if (tile == 'C')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BendUpLeft.png", false);
					file[i] = 'C';
				}
				if (tile == 'D')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BendUpRight.png", false);
					file[i] = 'D';
				}
				if (tile == 'E')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/StraightLeftRight.png", false);
					file[i] = 'E';
				}
				if (tile == 'F')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/StraightUpDown.png", false);
					file[i] = 'F';
				}
				if (tile == 'Z')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/Square.png", false);
					file[i] = 'Z';
				}
				if (tile == 'M')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/MUNCHIE.png", false);
					file[i] = 'M';
				}
				if (tile == 'P')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/Pacman.png", false);
					file[i] = 'P';
				}
				if (tile == 'X')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/Ghost.png", false);
					file[i] = 'X';
				}
				if (tile == 'H')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/Cherry.png", false);
					file[i] = 'H';
				}
			}
		}
	}
}

void Pacman::CheckEditor(Input::KeyboardState* state, Input::Keys editorKey)
{
	if (state->IsKeyDown(Input::Keys::F10))
	{
		editor = true;	
		_startGame = true;
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	//Pause Menu
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (state->IsKeyUp(Input::Keys::P)) {
		_pKeyDown = false;
	}
}

void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (state->IsKeyDown(Input::Keys::SPACE) && !_spaceKeyDown)
	{
		_spaceKeyDown = true;
		_startGame = !_startGame;
	}
}

void Pacman::CheckViewportCollision()
{

	if (roomClear == true)
	{
		//Right
		if (_pacman->_Position->X + _pacman->_SourceRect->Width > Graphics::GetViewportWidth()) {
			_pacman->_Position->X = (Graphics::GetViewportWidth() - Graphics::GetViewportWidth() - 32) + _pacman->_SourceRect->Width;
			roomClear = false;
			level++;
		}
		//Left
		if (_pacman->_Position->X - _pacman->_SourceRect->Width < Graphics::GetViewportWidth() - Graphics::GetViewportWidth() - 32) {
			_pacman->_Position->X = Graphics::GetViewportWidth() - _pacman->_SourceRect->Width;
			roomClear = false;
			level++;
		}
		//Up
		if (_pacman->_Position->Y - _pacman->_SourceRect->Height < Graphics::GetViewportHeight() - Graphics::GetViewportHeight() - 32) {
			_pacman->_Position->Y = Graphics::GetViewportHeight() - _pacman->_SourceRect->Height;
			roomClear = false;
			level++;
		}
		//Down
		if (_pacman->_Position->Y + _pacman->_SourceRect->Height > Graphics::GetViewportHeight()) {
			_pacman->_Position->Y = (Graphics::GetViewportHeight() - Graphics::GetViewportHeight() - 32) + _pacman->_SourceRect->Height;
			roomClear = false;
			level++;
		}
	}
	else if(roomClear==false)
	{
		//Right
		if (_pacman->_Position->X + _pacman->_SourceRect->Width > Graphics::GetViewportWidth()) {
			_pacman->_Position->X = Graphics::GetViewportWidth() - _pacman->_SourceRect->Width;
		}
		//Left
		if (_pacman->_Position->X - _pacman->_SourceRect->Width < Graphics::GetViewportWidth() - Graphics::GetViewportWidth() - 32) {
			_pacman->_Position->X = Graphics::GetViewportWidth() - Graphics::GetViewportWidth() ;
		}
		//Up
		if (_pacman->_Position->Y - _pacman->_SourceRect->Height < Graphics::GetViewportHeight() - Graphics::GetViewportHeight() - 32) {
			_pacman->_Position->Y = Graphics::GetViewportHeight() - Graphics::GetViewportHeight() ;
		}
		//Down
		if (_pacman->_Position->Y + _pacman->_SourceRect->Height > Graphics::GetViewportHeight()) {
			_pacman->_Position->Y = Graphics::GetViewportHeight()  - _pacman->_SourceRect->Height;
		}
	}
}

bool Pacman::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int leftPacman = x1;
	int left2 = x2;
	int rightPacman = x1 + width1;
	int right2 = x2 + width2;
	int topPacman = y1;
	int top2 = y2;
	int bottomPacman = y1 + height1;
	int bottom2 = y2 + height2;
	if (bottomPacman < top2)
	{
		return false;
	}
	if (topPacman > bottom2)
	{
		return false;
	}
	if (rightPacman < left2)
	{
		return false;
	}
	if (leftPacman > right2)
	{
		return false;
	}

	return true;
}

void Pacman::CreateEnemy(int x,int y)
{
	Texture2D* ghostTexture = new Texture2D();
	ghostTexture->Load("Textures/Enemies/Ghost.png", false);
	_ghost->position = new Vector2(x, y);
	_ghost->texture = ghostTexture;
	_ghost->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_ghost->collPos = new Vector2(x, y);
	
}

void Pacman::CreateLevel()
{
	//Load Tile Textures
	Texture2D* tileBendDownLeft = new Texture2D();
	tileBendDownLeft->Load("Textures/Level/BendDownleft.png", false);
	Texture2D* tileBendDownRight = new Texture2D();
	tileBendDownRight->Load("Textures/Level/BendDownRight.png", false);
	Texture2D* tileBendUpLeft = new Texture2D();
	tileBendUpLeft->Load("Textures/Level/BendUpleft.png", false);
	Texture2D* tileBendUpRight = new Texture2D();
	tileBendUpRight->Load("Textures/Level/BendUpRight.png", false);
	Texture2D* tileStraightLeftRight = new Texture2D();
	tileStraightLeftRight->Load("Textures/Level/StraightLeftRight.png", false);
	Texture2D* tileStraightUpDown = new Texture2D();
	tileStraightUpDown->Load("Textures/Level/StraightUpDown.png", false);
	Texture2D* tileBlank = new Texture2D();
	tileBlank->Load("Textures/Level/Blank.png", false);
	Texture2D* munchieTexture = new Texture2D();
	munchieTexture->Load("Textures/Food/Munchie.png", true);
	/*
	A = BendDownLeft
	B = BendDownRight
	C = BendUpLeft
	D = BendUpRight
	E = StraightLeftRight
	F = StraightUpDown
	M = Munchie
	H = Cherry
	Z = Empty
	X = Enemy
	P = Pacman
	N = Boss
	*/
	int width = Graphics::GetViewportWidth() / 32;
	int height = Graphics::GetViewportHeight() / 32;
	int i = 0;
	int count = 0;
	int munchieCount = 0;
	int x = 0, y = 0;
	ifstream level("Levels/1.txt");
	string line;

	//converts level file to char array
	while (!level.eof())
	{
		level >> levelArr[i];
		i++;
	}
	
	//gives info for tile struct
	for (int i = 0; i < cTileNum; i++)
	{
		_tile[i] = new Tile;
		_tile[i]->rect = new Rect(0.0f, 0.0f, cTileSize, cTileSize);
		_tile[i]->position = new Vector2(x,y);
		x += 32;
		if (Graphics::GetViewportWidth() <= x)
		{
			x = 0;
			y += 32;
		}
		if (levelArr[i]=='A')
		{
			_tile[i]->texture = tileBendDownLeft;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'B')
		{
			_tile[i]->texture = tileBendDownRight;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'C')
		{
			_tile[i]->texture = tileBendUpLeft;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'D')
		{
			_tile[i]->texture = tileBendUpRight;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'E')
		{
			_tile[i]->texture = tileStraightLeftRight;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'F')
		{
			_tile[i]->texture = tileStraightUpDown;
			_tile[i]->collision = 1;
		}
		if (levelArr[i] == 'Z')
		{
			_tile[i]->texture = tileBlank;
			_tile[i]->collision = 0;
		}
		if (levelArr[i] == 'M')
		{
			_tile[i]->texture = tileBlank;
			Pacman::SpawnMunchie(munchieTexture, x, y, munchieCount);
			munchieCount++;
			_tile[i]->collision = 0;
		}
		if (levelArr[i] == 'X')
		{
			_tile[i]->texture = tileBlank;
			_tile[i]->collision = 0;
			ghostCount++;
			Pacman::CreateEnemy(_tile[i]->position->X, _tile[i]->position->Y);
		}
		if (levelArr[i] == 'P')
		{
			_tile[i]->texture = tileBlank;
			_pacman->_Position = new Vector2(x, y);
			_pacmanColl->position = new Vector2(x, y);
			_tile[i]->collision = 0;
		}
		if (levelArr[i] == 'N')
		{
			_tile[i]->texture = tileBlank;
			_tile[i]->collision = 0;
		}
		if (levelArr[i] == 'H')
		{
			_tile[i]->texture = tileBlank;
			_tile[i]->collision = 0;
			Pacman::SpawnCherry(x, y);
		}
	}
	ifstream close();
}

void Pacman::Editor()
{
	string level;
	int strLocal;
	ofstream levelEdit;
	if (editInitial == false)
	{
		_editor->location = "Levels/X.txt";
		cout << "Enter A One Word File Name" << endl;
		cin >> _editor->fileName;
		cout << "A = BendDownLeft\nB = BendDownRight\nC = BendUpLeft\nD = BendUpRight\nE = StraightLeftRight\nF = StraightUpDown\nM = Munchie\nH = Cherry\nZ = Empty\nX = Enemy\nP = Pacman" << endl;
		cout << "Press S To Save The File When You're Ready" << endl;
	}

	if (levelSave == false && editInitial == false)
	{
		strLocal = _editor->location.find('X');
		_editor->location.replace(strLocal, 1, _editor->fileName);
		levelEdit.open(_editor->location);
		levelEdit.close();
		Texture2D* tileBlank = new Texture2D();
		tileBlank->Load("Textures/Level/Square.png", false);
		for (int i = 0; i < cTileNum; i++)
		{
			_tile[i]->texture = tileBlank;
		}
		cout << _editor->location << endl;
	}

	if (levelSave == true)
	{
		levelEdit.open(_editor->location, ios::app);
		cout << _editor->location << endl;
		//for (int i = 0; i < cTileNum; i++)
		{
		//	cout << file[i];
		}
		for (int i = 0; i < cTileNum; i++)
		{
			level = level + file[i];
		}
		levelEdit << level;
		levelEdit.close();
		exit(0);
	}
	
	editInitial = true;
}

void Pacman::EnemyCollision()
{
}

void Pacman::Immune()
{
	_pacman->immuneTimer++;
	if (_pacman->immuneTimer == 300)
	{
		_pacman->immune = false;
		_pacman->immuneTimer = 0;
	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	//Sprint
	if (_pacman->sprint==false)
	{
		if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			_pacman->sprint = true;
			_pacman->sprintAnim = true;
		}
	}
	if (_pacman->sprintAnim == true && _pacman->super == true)
	{
		_pacman->speedMulti = 4.0f;
	}
	else if(_pacman->sprintAnim == true)
	{
		_pacman->speedMulti = 2.0f;
	}
	else if (_pacman->super == true)
	{
		_pacman->speedMulti = 2.0f;
	}
	else
	{
		_pacman->speedMulti = 1.0f;
	}
	//Set Pacman Speed
	_pacman->speed = _cPacmanSpeed * elapsedTime * _pacman->speedMulti;
	for (int i = 0; i < cTileNum; i++)
	{
		if (_tile[i]->collision==1)
		{
			if (_pacman->_Direction == 2 || _pacman->_Direction == 0)
			{
				if (TileCollisionCheck(_pacmanColl->position->X, _pacmanColl->position->Y, 1, 30,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					_pacman->speed = 0;
				}
			}
			if (_pacman->_Direction == 3 || _pacman->_Direction == 1)
			{
				if (TileCollisionCheck(_pacmanColl->position->X, _pacmanColl->position->Y, 30, 1,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					_pacman->speed = 0;
				}
			}
		}
	}

	// Moves Character Left
	if (state->IsKeyDown(Input::Keys::A)) {
		_pacman->_Position->X -= _pacman->speed;
		_pacman->_Direction = 2;
		_pacmanColl->position->Y = _pacman->_Position->Y+1;
		_pacmanColl->position->X = _pacman->_Position->X;
	}
	// Moves Character Right
	else if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->_Position->X += _pacman->speed;
		_pacman->_Direction = 0;
		_pacmanColl->position->Y = _pacman->_Position->Y+1;
		_pacmanColl->position->X = _pacman->_Position->X + 32;
	}
	// Moves Character Up
	else if (state->IsKeyDown(Input::Keys::W)) {
		_pacman->_Position->Y -= _pacman->speed;
		_pacman->_Direction = 3;
		_pacmanColl->position->Y = _pacman->_Position->Y;
		_pacmanColl->position->X = _pacman->_Position->X+1;
	}
	// Moves Character Down
	else if (state->IsKeyDown(Input::Keys::S)) {
		_pacman->_Position->Y += _pacman->speed;
		_pacman->_Direction = 1;
		_pacmanColl->position->Y = _pacman->_Position->Y+32;
		_pacmanColl->position->X = _pacman->_Position->X+1;
	}
	if (state->IsKeyDown(Input::Keys::ESCAPE))
	{
		exit(0);
	};
	
	if (state->IsKeyDown(Input::Keys::DELETEKEY))
	{
		debug = !debug;
	}
	if (editor == true)
	{
		if (mouseState->LeftButton == Input::ButtonState::PRESSED)
		{
			_editorColl->position->X = mouseState->X;
			_editorColl->position->Y = mouseState->Y;
		}
		if (state->IsKeyDown(Input::Keys::A))
			tile = 'A';
		if (state->IsKeyDown(Input::Keys::B))
			tile = 'B';
		if (state->IsKeyDown(Input::Keys::C))
			tile = 'C';
		if (state->IsKeyDown(Input::Keys::D))
			tile = 'D';
		if (state->IsKeyDown(Input::Keys::E))
			tile = 'E';
		if (state->IsKeyDown(Input::Keys::F))
			tile = 'F';
		if (state->IsKeyDown(Input::Keys::Z))
			tile = 'Z';
		if (state->IsKeyDown(Input::Keys::M))
			tile = 'M';
		if (state->IsKeyDown(Input::Keys::X))
			tile = 'X';
		if (state->IsKeyDown(Input::Keys::P))
			tile = 'P';
		if (state->IsKeyDown(Input::Keys::N))
			tile = 'N';
		if (state->IsKeyDown(Input::Keys::H))
			tile = 'H';
		if (state->IsKeyDown(Input::Keys::S))
			levelSave = true;
		
	}
	//Debug Keys
	if (debug == true)
	{
		if (state->IsKeyDown(Input::Keys::F1)) {
			roomClear = true;
		}
		if (state->IsKeyDown(Input::Keys::F2)) {
			_pacman->health = 0;
		}
		if (mouseState->LeftButton == Input::ButtonState::PRESSED)
		{
			_pacman->_Position->X = mouseState->X;
			_pacman->_Position->Y = mouseState->Y;
		}
	}
}

void Pacman::Restart(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_pacman->_Direction = 0;
		_pacman->_CurrentFrameTime = 0;
		_pacman->_Frame = 0;
		_pacman->sprint = false;
		_pacman->sprintAnim = false;
		_pacman->super = false;
		_pacman->superTimer = 0;
		_pacman->dead = false;
		_pacman->immune = false;
		_pacman->health = 100;
		score = 0;
		level = 1;
		Pacman::CreateLevel();
		_paused = false;
	}
}

void Pacman::SpawnCherry(int x, int y)
{
	_cherry->_Rect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->_position = new Vector2(x, y);
}

void Pacman::SpawnMunchie(Texture2D* munchieTexture,int x,int y, int num)
{
	_munchies[num]->_Texture = munchieTexture;
	_munchies[num]->_Rect = new Rect(0.0f, 0.0f, 12, 12);
	_munchies[num]->_position = new Vector2(x+10, y+10);
}

void Pacman::Sprint()
{
	_pacman->sprintCooldown++;
	if (_pacman->sprintCooldown == 60)
	{
		_pacman->speedMulti = 1.0f;
		_pacman->sprintAnim = false;
	}
	if (_pacman->sprintCooldown == 180)
	{
		_pacman->sprint = false;
		_pacman->sprintCooldown = 0;
	}
}

void Pacman::Super()
{
	scoreMulti = 2.0f;
	_pacman->superTimer++;
	if (_pacman->superTimer == 300)
	{
		_pacman->super = false;
		_pacman->superTimer = 0;
		scoreMulti = 1.0f;
	}
}

bool Pacman::TileCollisionCheck(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2)
{
	float leftPacman = x1;
	float left2 = x2;
	float rightPacman = x1 + width1;
	float right2 = x2 + width2;
	float topPacman = y1;
	float top2 = y2;
	float bottomPacman = y1 + height1;
	float bottom2 = y2 + height2;
	if (bottomPacman < top2)
	{
		return false;
	}
	if (topPacman > bottom2)
	{
		return false;
	}
	if (rightPacman < left2)
	{
		return false;
	}
	if (leftPacman > right2)
	{
		return false;
	}

	return true;
}

void Pacman::UpdateGhost(Enemy* ghost, int elapsedTime)
{
	//Movement
	//Right
	for (int i = 0; i < cTileNum; i++)
	{
		if (_tile[i]->collision==true)
		{
			if (ghost->direction == 0 || ghost->direction == 2)
			{
				if (TileCollisionCheck(ghost->collPos->X, ghost->collPos->Y, 1, 32,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					ghost->direction = rand() % 4;
					break;
				}
			}
			if (ghost->direction == 1 || ghost->direction == 3)
			{
				if (TileCollisionCheck(ghost->collPos->X, ghost->collPos->Y, 32, 1,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					ghost->direction = rand() % 4;
					break;
				}
			}
		}
	}
	if (rand()%10000 >= 9500)
	{
		ghost->direction = rand() % 4;
	}

	//Right
	if (ghost->direction == 0)
	{
		ghost->position->X += ghost->speed * elapsedTime;
		ghost->collPos->X = ghost->position->X + 32;
		ghost->collPos->Y = ghost->position->Y;
	}
	//Left
	else if (ghost->direction == 2)
	{
		ghost->position->X -=ghost->speed * elapsedTime;
		ghost->collPos->X = ghost->position->X;
		ghost->collPos->Y = ghost->position->Y;
	}
	//Up
	else if (ghost->direction == 1)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
		ghost->collPos->X = ghost->position->X;
		ghost->collPos->Y = ghost->position->Y;
	}
	//Down
	else if (ghost->direction == 3)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
		ghost->collPos->X = ghost->position->X;
		ghost->collPos->Y = ghost->position->Y + 32;
	}
}

void Pacman::UpdateLevel()
{
	ifstream level("Levels/1.txt");
	string line;
	int i = 0, x = 0, y = 0,munchieCount=0;
	//converts level file to char array
	while (!level.eof())
	{
		level >> levelArr[i];
		i++;
	}

	//gives info for tile struct
	for (int i = 0; i < cTileNum; i++)
	{
		_tile[i] = new Tile;
		_tile[i]->rect = new Rect(0.0f, 0.0f, cTileSize, cTileSize);
		_tile[i]->position = new Vector2(x, y);
		x += 32;
		if (Graphics::GetViewportWidth() <= x)
		{
			x = 0;
			y += 32;
		}
		
		if (levelArr[i] == 'M')
		{
			_munchies[munchieCount]->_position->X = x;
			_munchies[munchieCount]->_position->Y = y;
			munchieCount++;
		}
		if (levelArr[i] == 'X')
		{
			/*ghostCount++;
			_ghost[i].position->X = x;
			_ghost[i].position->Y = y;*/
		}
		if (levelArr[i] == 'P')
		{
			_pacman->_Position = new Vector2(x, y);
			_pacmanColl->position = new Vector2(x, y);
		}
		if (levelArr[i] == 'H')
		{
			_cherry->_position->X = x;
			_cherry->_position->Y = y;
		}
	}
	ifstream close();
}

void Pacman::UpdateMunchie(Food* munchie, int elapsedTime)
{
	//Animates Munchie

	munchie->frameTime += elapsedTime;

	if (munchie->frameTime > _cMunchieFrameTime)
	{
		munchie->_FrameCount++;

		if (munchie->_FrameCount > 1)
			munchie->_FrameCount = 0;

		munchie->frameTime = 0;
	}

	munchie->_Rect->X = munchie->_Rect->Width * munchie->_FrameCount;

}

void Pacman::UpdatePacman(int elapsedTime)
{
	//Pacman Frame Time
	_pacman->_CurrentFrameTime += elapsedTime;
	if (_pacman->_CurrentFrameTime > _cPacmanFrameTime)
	{
		_pacman->_Frame++;
		if (_pacman->_Frame > 1)
			_pacman->_Frame = 0;
		_pacman->_CurrentFrameTime = 0;
	}
	//Pacman Source Rect
	//Changes Pacmans Direction
	
	if (_pacman->sprintAnim == true && _pacman->super == true)
	{
		_pacman->_SourceRect->Y = _pacman->_SourceRect->Height * _pacman->_Direction + 256;
	}
	else if (_pacman->sprintAnim == true)
	{
		_pacman->_SourceRect->Y = _pacman->_SourceRect->Height * _pacman->_Direction +128;
	}
	else if (_pacman->super == true)
	{
		_pacman->_SourceRect->Y = _pacman->_SourceRect->Height * _pacman->_Direction + 384;
	}
	else
	{
		_pacman->_SourceRect->Y = _pacman->_SourceRect->Height * _pacman->_Direction;
	}
	//Animates Pacman

	_pacman->_SourceRect->X = _pacman->_SourceRect->Width * _pacman->_Frame;
	if (_pacman->health == 0)
	{
		_pacman->dead = true;
	}
}

