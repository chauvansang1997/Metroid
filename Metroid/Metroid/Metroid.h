#pragma once
#include "GameManager.h"
#include "Samus.h"
#include "Skree.h"

class Metroid : public GameManager
{
private:
	Samus *samus;
	TextureManager *textureManager;
	SpriteManager *spriteManger;

	// Enemy
	Skree *skree;

public:
	Metroid();
	~Metroid();


	void initialize(HWND hwnd);
	void update(float dt);      // must override pure virtual from Game
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();
};

