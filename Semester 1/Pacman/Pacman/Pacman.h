#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 12

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Structs
struct Food
{
	int _FrameCount;
	Rect* _Rect;
	Texture2D* _Texture;
	int _CurrentFrameTime;
	Vector2* _position;
	int frameTime;
};

struct Enemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct Player
{
	Vector2* _Position;
	Rect* _SourceRect;
	Texture2D* _Texture;
	int _Direction;
	int _Frame;
	int _CurrentFrameTime;
	float speedMulti;
	bool sprint;
	bool sprintAnim;
	bool super;
	bool immune;
	int immuneTimer;
	int superTimer;
	int sprintCooldown;
	int health;
	float speed;
	bool dead;
};

struct PlayerColl
{
	Vector2* position;
	Rect* rect;
	Texture2D* texture;
};

struct Tile
{
	Rect* rect;
	Texture2D* texture;
	Vector2* position;
	int collision;
};


class Pacman : public Game
{
private:
	//Constant Variables
	const int _cMunchieFrameTime;
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int cSpawnDistance;
	const int cTileNum;
	const int cTileSize;

	// Position for String
	Vector2* _stringPosition;

	Food* _cherry;
	Food* _munchies[MUNCHIECOUNT];
	Enemy* _ghost;
	Player* _pacman;
	PlayerColl* _pacmanColl;
	Tile* _tile[768];

	//data for menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _menuPosition;
	bool _paused;
	bool _pKeyDown;
	bool _startGame;
	bool _spaceKeyDown;

	int score;
	int highScore;
	float scoreMulti;
	int level;
	int ghostCount;
	bool debug;

	char levelArr[768];

	bool pacmanProj;
	
	//Debug Variables
	bool roomClear;

	//Pacman Functions
	void CheckCollision();
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	void CheckViewportCollision();
	bool CollisionCheck(int x1, int y1, int width1, int height1,
						int x2, int y2, int width2, int height2);
	void CreateEnemy(int x, int y);
	void CreateLevel();
	void EnemyCollision();
	void Immune();
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void Restart(Input::KeyboardState* state, Input::Keys pauseKey);
	void SpawnCherry(int x, int y);
	void SpawnMunchie(Texture2D* munchieTexture, int x,int y,int num);
	void Sprint();
	void Super();
	bool TileCollisionCheck(float x1, float y1, float width1, float height1,
		float x2, float y2, float width2, float height2);
	void UpdateGhost(Enemy* ghost, int elapsedTime);
	void UpdateLevel();
	void UpdateMunchie(Food* munchie, int elapsedTime);
	void UpdatePacman(int elapsedTime);


public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};