#include "Pacman.h"

#include <sstream>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv),_cPacmanSpeed(0.1f),_cPacmanFrameTime(250),_cMunchieFrameTime(500),cSpawnDistance(64),cTileNum(768),cTileSize(32),_cGhostFrameTime(250){
	//Initialise Member Variables
	_ghost.resize(0);
	_cherry.resize(0);
	_munchies.resize(0);
	srand(time(NULL));
	_editor = new LevelEditor();
	_pacman = new Player();
	_editorColl = new PlayerColl();
	_pacmanColl = new PlayerColl();


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
	levelLoaded = false;
	playSound = true;

	_collect = new SoundEffect();
	_wa = new SoundEffect();
	_ka = new SoundEffect();
	_coll = new SoundEffect();

	for (int i = 0; i < cTileNum; i++)
	{
		file[i] = 'Z';
	}
	ghostCount = 0;
	munchieCount = 0;
	levelSave = false;
	level = 1;
	counter = 0;
	win = false;
	pacmanProj = false;
	roomClear = false;
	score = 0;
	scoreMulti = 1.0f;
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//Clean Up Pointers In Structure
	delete _pacman->_Texture;
	delete _pacman->_SourceRect;
	delete _pacman->_Position;
	int i = 0;
	for (vector<Food>::iterator it = _munchies.begin(); it != _munchies.end(); it++)
	{
		delete _munchies[i]._Rect;
		delete _munchies[i]._position;
		i++;
	}	
	i = 0;
	for (vector<Food>::iterator it = _cherry.begin(); it != _cherry.end(); it++)
	{
		delete _cherry[i]._position;
		delete _cherry[i]._Texture;
		i++;
	}
	i = 0;
	for (vector<Enemy>::iterator it = _ghost.begin(); it != _ghost.end() ; it++)
	{
		delete _ghost[i].collPos;
		delete _ghost[i].collRect;
		delete _ghost[i].position;
		delete _ghost[i].sourceRect;
		delete _ghost[i].texture;
		i++;
	}
	_munchies.clear();
	_cherry.clear();
	_ghost.clear();

	delete _wa;
	delete _ka;
	delete _coll;
	delete _collect;

	delete _munchies[0]._Texture;

	delete _pacmanColl->rect;
	delete _pacmanColl->texture;
	delete _pacmanColl->position;
	//Clean Up Pacman Structure Pointer
	delete _pacman;
	delete _pacmanColl;

	delete _menuBackground;
	delete _menuRectangle;
}

void Pacman::LoadContent()
{
	ifstream highscoreDoc;
	highscoreDoc.open("Score/score.txt");
	getline(highscoreDoc, scoreStr);
	stringstream scoreStream(scoreStr);
	scoreStream >> highScore;
	highscoreDoc.close();
	// Load Pacman
	_pacman->_Texture = new Texture2D();
	_pacman->_Texture->Load("Textures/Pacman.tga", false);
	_pacman->_SourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacmanColl->rect = new Rect(0.0f, 0.0f, 1, 1);
	_pacmanColl->texture = new Texture2D();
	_pacmanColl->texture->Load("texture", false);

	_editorColl->rect = new Rect(0.0f, 0.0f, 1, 1);
	_editorColl->position = new Vector2(0.0f, 0.0f);
	
	_wa->Load("Audio/wa.wav");
	_ka->Load("Audio/ka.wav");
	_coll->Load("Audio/PacmanColl.wav");
	_collect->Load("Audio/PacmanCollect.wav");

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
	
	if (levelLoaded == false)
	{
		levelLoc = Pacman::ChooseLevel(keyboardState);
		Pacman::CreateLevel(levelLoc);
	}
	Pacman::CheckEditor(keyboardState, Input::Keys::F10);
	//Start Game
	if (levelLoaded == true)
	{
		Pacman::CheckStart(keyboardState, Input::Keys::SPACE);
	}
	//Checks If The Game Has Started 
	//_startGame = true;
	if (_startGame == true)
	{
		//Checks If Game Is Paused Before Updating Anything Else
		Pacman::CheckPaused(keyboardState, Input::Keys::P);
		if (_pacman->dead || win == true)
		{
			Pacman::Restart(keyboardState, Input::Keys::SPACE);
		}
		if (win == false)
		{
			if (!_paused)
			{
				Pacman::Input(elapsedTime, keyboardState, mouseState);
				if (editor == false)
				{
					Pacman::UpdateMunchie(elapsedTime);

					Pacman::UpdatePacman(elapsedTime);

					Pacman::UpdateGhost(elapsedTime);

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
				}
		
				if (editor == true)
				{
					Pacman::Editor();
				}

				//CollisionChecks
				Pacman::CheckCollision();

				if (munchieCount == 0)
				{
					win = true;
				}
				//Confine Pacman To Screen
				Pacman::CheckViewportCollision();
				if (score > highScore)
				{
					highScore = score;
					ofstream highscoreDoc("Score/score.txt");
					highscoreDoc << highScore;
					highscoreDoc.close();
				}
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
	if (levelLoaded == true)
	{
		if (editor == false)
		{
			for (int i = 0; i < cTileNum; i++)
			{
				SpriteBatch::Draw(_tile[i]->texture, _tile[i]->position, _tile[i]->rect);
			}
			// Allows us to easily create a string

			stream << "Health: " << _pacman->health << "     Score: " << score << "     High Score: " << highScore;

			//Draw Munchie
			int i = 0;
			for (vector<Food>::iterator it = _munchies.begin(); it != _munchies.end(); it++)
			{
				SpriteBatch::Draw(_munchies[i]._Texture, _munchies[i]._position, _munchies[i]._Rect);
				i++;
			}

			//Draw Cherry
			i = 0;
			for (vector<Food>::iterator it = _cherry.begin(); it != _cherry.end(); it++)
			{
				SpriteBatch::Draw(_cherry[i]._Texture, _cherry[i]._position, _cherry[i]._Rect);
				i++;
			}


			//Draw Enemies
			for (counter = 0, it = _ghost.begin(); it != _ghost.end(); it++, counter++)
			{
				SpriteBatch::Draw(_ghost[counter].texture, _ghost[counter].position, _ghost[counter].sourceRect);
				//SpriteBatch::Draw(_pacmanColl->texture, _ghost[counter].collPos, _ghost[counter].collRect);
			}


			//Draw Pacman
			if (!_pacman->dead)
			{
				SpriteBatch::Draw(_pacman->_Texture, _pacman->_Position, _pacman->_SourceRect);
			}
		}
	}
	if (editor == true)
	{
		for (int i = 0; i < cTileNum; i++)
		{
			SpriteBatch::Draw(_tile[i]->texture, _tile[i]->position, _tile[i]->rect);
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
		if (win == true)
		{
			std::stringstream menuStream;
			menuStream << "CONGRATULATIONS YOU WON \nPress Space To Restart";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
		// Draws Start Game
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		if (!_startGame && levelLoaded == true)
		{
			std::stringstream menuStream;
			menuStream << "PRESS SPACE TO START";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
		if (levelLoaded == false)
		{
			std::stringstream menuStream;
			menuStream << "PRESS A NUMBER BETWEEN 1 AND 5 TO LOAD A LEVEL";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
	}
	

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::CheckCollision()
{
	//Munchie Collision
	int i = 0;
	for (vector<Food>::iterator it = _munchies.begin(); it != _munchies.end(); it++)
	{
		if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
			_munchies[i]._position->X, _munchies[i]._position->Y, _munchies[i]._Rect->Width, _munchies[i]._Rect->Height))
		{
			Audio::Play(_collect);
			_munchies[i]._position = new Vector2(-100, -100);
			score += (100 * scoreMulti);
			munchieCount--;
		}
		i++;
	}
	//Cherry Collision
	i = 0;
	for (vector<Food>::iterator it = _cherry.begin(); it != _cherry.end(); it++)
	{
		if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
			_cherry[i]._position->X, _cherry[i]._position->Y, _cherry[i]._Rect->Width, _cherry[i]._Rect->Height))
		{
			_cherry[i]._position = new Vector2(-100, -100);
			score += (1000 * scoreMulti);
			_pacman->super = true;
			Audio::Play(_collect);
		}
		i++;
	}
	for (counter = 0, it = _ghost.begin(); it != _ghost.end(); it++,counter++)
	{
		if (CollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height,
			_ghost[counter].position->X, _ghost[counter].position->Y, _ghost[counter].sourceRect->Width, _ghost[counter].sourceRect->Height))
		{
			if (_pacman->immune)
			{
			}
			else if (_pacman->super == true || _pacman->sprint == true)
			{
				_ghost[counter].position = new Vector2(-100, -100);
				score += (2500 * scoreMulti);
			}
			else if (!_pacman->immune)
			{
				_pacman->health -= 10;
				_pacman->immune = true;
			}
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
				if (tile == 'G')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/3WayLeft.png", false);
					file[i] = 'G';
				}
				if (tile == 'I')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/3WayUp.png", false);
					file[i] = 'I';
				}
				if (tile == 'J')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/3WayRight.png", false);
					file[i] = 'J';
				}
				if (tile == 'K')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/3WayDown.png", false);
					file[i] = 'K';
				}
				if (tile == 'L')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/4Way.png", false);
					file[i] = 'L';
				}
				if (tile == 'W')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BlockLeft.png", false);
					file[i] = 'W';
				}
				if (tile == 'U')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BlockRight.png", false);
					file[i] = 'U';
				}
				if (tile == 'Y')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BlockUp.png", false);
					file[i] = 'Y';
				}
				if (tile == 'V')
				{
					_tile[i]->texture = new Texture2D();
					_tile[i]->texture->Load("Textures/Level/BlockDown.png", false);
					file[i] = 'V';
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

string Pacman::ChooseLevel(Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::NUMPAD1))
	{
		return string("Levels/1.txt");
	}
	if (state->IsKeyDown(Input::Keys::NUMPAD2))
	{
		return string("Levels/2.txt");
	}
	if (state->IsKeyDown(Input::Keys::NUMPAD3))
	{
		return string("Levels/3.txt");
	}
	if (state->IsKeyDown(Input::Keys::NUMPAD4))
	{
		return string("Levels/4.txt");
	}
	if (state->IsKeyDown(Input::Keys::NUMPAD5))
	{
		return string("Levels/5.txt");
	}
	return string();
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
	ghostCount++;
	_ghost.push_back(Enemy());
	_ghost.back().position = new Vector2(x, y);
	_ghost.back().texture = ghostTexture;
	_ghost.back().sourceRect = new Rect(0.0f, 0.0f, 30, 30);
	_ghost.back().collPos = new Vector2(x, y);
	_ghost.back().collRect = new Rect(0.0f, 0.0f, 1, 1);
	_ghost.back().direction = 0;
	_ghost.back().speed = 0.2f;
	_ghost.back().currentFrameTime = 0;
	_ghost.back().frameTime = 0;
	_ghost.back().frameCount = 0;
}

void Pacman::CreateLevel(string location)
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
	Texture2D* tile3WayLeft = new Texture2D();
	tile3WayLeft->Load("Textures/Level/3WayLeft.png", false);
	Texture2D* tile3WayUp = new Texture2D();
	tile3WayUp->Load("Textures/Level/3WayUp.png", false);
	Texture2D* tile3WayRight = new Texture2D();
	tile3WayRight->Load("Textures/Level/3WayRight.png", false);
	Texture2D* tile3WayDown = new Texture2D();
	tile3WayDown->Load("Textures/Level/3WayDown.png", false);
	Texture2D* tile4Way = new Texture2D();
	tile4Way->Load("Textures/Level/4Way.png", false);
	Texture2D* tileBlank = new Texture2D();
	tileBlank->Load("Textures/Level/Blank.png", false);
	Texture2D* tileBlockLeft = new Texture2D();
	tileBlockLeft->Load("Textures/Level/BlockLeft.png", false);
	Texture2D* tileBlockRight = new Texture2D();
	tileBlockRight->Load("Textures/Level/BlockRight.png", false);
	Texture2D* tileBlockUp = new Texture2D();
	tileBlockUp->Load("Textures/Level/BlockUp.png", false);
	Texture2D* tileBlockDown = new Texture2D();
	tileBlockDown->Load("Textures/Level/BlockDown.png", false);
	Texture2D* munchieTexture = new Texture2D();
	munchieTexture->Load("Textures/Food/Munchie.png", true);

	int width = Graphics::GetViewportWidth() / 32;
	int height = Graphics::GetViewportHeight() / 32;
	int i = 0;
	int count = 0;
	int munchieCount = 0;
	int x = 0, y = 0;
	
	ifstream level(location);
	if (level.is_open())
	{
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
			_tile[i]->position = new Vector2(x, y);
			x += 32;
			if (Graphics::GetViewportWidth() <= x)
			{
				x = 0;
				y += 32;
			}
			if (levelArr[i] == 'A')
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
			if (levelArr[i] == 'G')
			{
				_tile[i]->texture = tile3WayLeft;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'I')
			{
				_tile[i]->texture = tile3WayUp;
				_tile[i]->collision = 1;

			}
			if (levelArr[i] == 'J')
			{
				_tile[i]->texture = tile3WayRight;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'K')
			{
				_tile[i]->texture = tile3WayDown;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'L')
			{
				_tile[i]->texture = tile4Way;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'W')
			{
				_tile[i]->texture = tileBlockLeft;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'U')
			{
				_tile[i]->texture = tileBlockRight;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'Y')
			{
				_tile[i]->texture = tileBlockUp;
				_tile[i]->collision = 1;
			}
			if (levelArr[i] == 'V')
			{
				_tile[i]->texture = tileBlockDown;
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
				Pacman::SpawnMunchie(munchieTexture, _tile[i]->position->X, _tile[i]->position->Y, munchieCount);
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
				_pacman->_Position = new Vector2(_tile[i]->position->X, _tile[i]->position->Y);
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
				Pacman::SpawnCherry(_tile[i]->position->X, _tile[i]->position->Y);
			}
		}
		ifstream close();
		levelLoaded = true;
	}
	
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
		cout << "A = BendDownLeft\nB = BendDownRight\nC = BendUpLeft\nD = BendUpRight\nE = StraightLeftRight\nF = StraightUpDown\nG = 3WayLeft\nI = 3WayUp\nJ = 3WayRight\nK = 3WayDown\nL = 4Way\nW = BlockLeft\nU = BlockRight\nY = BlockUp\nV = BlockDown\nM = Munchie\nH = Cherry\nZ = Empty\nX = Enemy\nP = Pacman" << endl;
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
				if (TileCollisionCheck(_pacmanColl->position->X, _pacmanColl->position->Y, 1, 26,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					//Audio::Play(_coll);
					_pacman->speed = 0;
				}
			}
			if (_pacman->_Direction == 3 || _pacman->_Direction == 1)
			{
				if (TileCollisionCheck(_pacmanColl->position->X, _pacmanColl->position->Y, 26, 1,
					_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
				{
					//Audio::Play(_coll);
					_pacman->speed = 0;
				}
			}
		}
	}

	// Moves Character Left
	if (state->IsKeyDown(Input::Keys::A)) {
		_pacman->_Position->X -= _pacman->speed;
		_pacman->_Direction = 2;
		_pacmanColl->position->Y = _pacman->_Position->Y+3;
		_pacmanColl->position->X = _pacman->_Position->X;
	}
	// Moves Character Right
	else if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->_Position->X += _pacman->speed;
		_pacman->_Direction = 0;
		_pacmanColl->position->Y = _pacman->_Position->Y+3;
		_pacmanColl->position->X = _pacman->_Position->X + 32;
	}
	// Moves Character Up
	else if (state->IsKeyDown(Input::Keys::W)) {
		_pacman->_Position->Y -= _pacman->speed;
		_pacman->_Direction = 3;
		_pacmanColl->position->Y = _pacman->_Position->Y;
		_pacmanColl->position->X = _pacman->_Position->X+3;
	}
	// Moves Character Down
	else if (state->IsKeyDown(Input::Keys::S)) {
		_pacman->_Position->Y += _pacman->speed;
		_pacman->_Direction = 1;
		_pacmanColl->position->Y = _pacman->_Position->Y+32;
		_pacmanColl->position->X = _pacman->_Position->X+3;
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
		if (state->IsKeyDown(Input::Keys::G))
			tile = 'G';
		if (state->IsKeyDown(Input::Keys::I))
			tile = 'I';
		if (state->IsKeyDown(Input::Keys::J))
			tile = 'J';
		if (state->IsKeyDown(Input::Keys::K))
			tile = 'K';
		if (state->IsKeyDown(Input::Keys::L))
			tile = 'L';
		if (state->IsKeyDown(Input::Keys::W))
			tile = 'W';
		if (state->IsKeyDown(Input::Keys::U))
			tile = 'U';
		if (state->IsKeyDown(Input::Keys::Y))
			tile = 'Y';
		if (state->IsKeyDown(Input::Keys::V))
			tile = 'V';
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
		win = false;
		_startGame = false;
		_ghost.clear();
		_munchies.clear();
		_cherry.clear();
		levelLoaded = false;
		Pacman::CreateLevel(levelLoc);
		_paused = false;
		_spaceKeyDown = false;
	}
}

void Pacman::SpawnCherry(int x, int y)
{
	_cherry.push_back(Food());
	_cherry.back()._Rect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry.back()._position = new Vector2(x, y);
	_cherry.back()._Texture = new Texture2D();
	_cherry.back()._Texture->Load("Textures/Food/Cherry.png", true);
}

void Pacman::SpawnMunchie(Texture2D* munchieTexture,int x,int y, int num)
{
	munchieCount++;
	_munchies.push_back(Food());
	_munchies.back()._Texture = munchieTexture;
	_munchies.back()._Rect = new Rect(0.0f, 0.0f, 12, 12);
	_munchies.back()._position = new Vector2(x+10, y+10);
	_munchies.back()._CurrentFrameTime = 0;
	_munchies.back()._FrameCount = rand() % 1;
	_munchies.back().frameTime = rand() % 500 + 50;

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
	_pacman->immune = false;
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

void Pacman::UpdateGhost(int elapsedTime)
{
	for (counter = 0, it = _ghost.begin();it != _ghost.end(); it++, counter++)
	{
		_ghost[counter].speed = 0.2f;
		for (int i = 0; i < cTileNum; i++)
		{
			if (_tile[i]->collision == 1)
			{
				if (_ghost[counter].direction == 0 || _ghost[counter].direction == 2)
				{
					if (TileCollisionCheck(_ghost[counter].collPos->X, _ghost[counter].collPos->Y, 1, _ghost[counter].sourceRect->Height - 2,
						_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
					{
						_ghost[counter].direction = rand() % 4;
						_ghost[counter].speed = 0;
						break;
					}
				}
				if (_ghost[counter].direction == 1 || _ghost[counter].direction == 3)
				{
					if (TileCollisionCheck(_ghost[counter].collPos->X, _ghost[counter].collPos->Y, _ghost[counter].sourceRect->Width - 2, 1,
						_tile[i]->position->X, _tile[i]->position->Y, _tile[i]->rect->Width, _tile[i]->rect->Height))
					{
						_ghost[counter].direction = rand() % 4;
						_ghost[counter].speed = 0;
						break;
					}
				}
			}
		}

		if (rand() % 10000 >= 9500)
		{
			_ghost[counter].direction = rand() % 4;
		}

		//Right
		if (_ghost[counter].direction == 0)
		{
			_ghost[counter].position->X += _ghost[counter].speed * elapsedTime;
			_ghost[counter].collPos->X = _ghost[counter].position->X + _ghost[counter].sourceRect->Width;
			_ghost[counter].collPos->Y = _ghost[counter].position->Y-1;
		}
		//Left
		else if (_ghost[counter].direction == 2)
		{
			_ghost[counter].position->X -= _ghost[counter].speed * elapsedTime;
			_ghost[counter].collPos->X = _ghost[counter].position->X;
			_ghost[counter].collPos->Y = _ghost[counter].position->Y-1;
		}
		//Up
		else if (_ghost[counter].direction == 1)
		{
			_ghost[counter].position->Y -= _ghost[counter].speed * elapsedTime;
			_ghost[counter].collPos->X = _ghost[counter].position->X+1;
			_ghost[counter].collPos->Y = _ghost[counter].position->Y;
		}
		//Down
		else if (_ghost[counter].direction == 3)
		{
			_ghost[counter].position->Y += _ghost[counter].speed * elapsedTime;
			_ghost[counter].collPos->X = _ghost[counter].position->X+1;
			_ghost[counter].collPos->Y = _ghost[counter].position->Y + _ghost[counter].sourceRect->Height;
		}
		_ghost[counter].frameTime += elapsedTime;
		if (_ghost[counter].frameTime > _cGhostFrameTime)
		{
			_ghost[counter].frameCount++;
			if (_ghost[counter].frameCount > 3)
				_ghost[counter].frameTime = 0;
			_ghost[counter].frameTime = 0;			
		}

		_ghost[counter].sourceRect->X = _ghost[counter].sourceRect->Width * _ghost[counter].frameCount;
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	//Animates Munchie
	int count = 0;
	for (vector<Food>::iterator it = _munchies.begin(); it !=_munchies.end(); it++)
	{
		_munchies[count].frameTime += elapsedTime;

		if (_munchies[count].frameTime > _cMunchieFrameTime)
		{
			_munchies[count]._FrameCount++;

			if (_munchies[count]._FrameCount > 1)
				_munchies[count]._FrameCount = 0;

			_munchies[count].frameTime = 0;
		}

		_munchies[count]._Rect->X = _munchies[count]._Rect->Width * _munchies[count]._FrameCount;
		count++;
	}

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
		playSound = true;
	}
	if (_pacman->_Frame == 0 && playSound == true)
	{
		Audio::Play(_wa);
		playSound = false;
	}
	else if (_pacman->_Frame == 1 && playSound == true)
	{
		Audio::Play(_ka);
		playSound = false;
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

