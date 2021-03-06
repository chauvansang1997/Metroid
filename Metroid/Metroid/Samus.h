#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Input.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "GameError.h"
#include "Animation.h"
#include "BaseObject.h"
#include "BulletPool.h"
#include "BoomBombPool.h"
#include "RocketPool.h"
#include <list>
#include "Text.h"
#include <map>
#define SAMUS_MIN_SPEED_Y 70
#define SAMUS_VERLOCITY_X 100
#define ACELERATE 70
#define SAMUS_MAX_SPEED_Y 170
#define SAMUS_V0_FALL_Y -60
#define SAMUS_VELOCITY_JUMP_X 70

#define MIN_JUMP  32.0f
#define MAX_JUMP 94.0f

#define TIME_SHOOTING 0.15f
#define TIME_SHOOTING_ROCKET 0.3f

#define MAX_HEIHT 32

#define WIDTH_RUN 20
#define HEIGHT_RUN 32

#define HEIGHT_JUMP 25
#define WIDTH_JUMP 18

#define ROLL_WIDTH 10
#define ROLL_HEIGHT 10

#define JUMP_HEIGHT 24
#define JUMP_WIDTH 10

#define ACCELERATE_Y -140.0f
#define ACCELERATE_X 50.0f

#define OFFSET_JUMP 11
#define OFFSET_RUN 5
#define OFFSET_UP 5
#define OFFSET_ACROBAT 2

#define TIME_TO_ACROBAT 0.1f

#define OFFSET_STAND 19
#define WIDTH_COLLISION 10
#define HEIGHT_COLLISION 30
#define OFFSET_COLLISION_X 0
#define OFFSET_COLLISION_Y 4
#define OFFSET_ROLLING 6.0f

#define TIME_DEC_HEALTH_FIRE 1



class Samus: public BaseObject
{
private:
	Input* input;
	TextureManager* textureManager;
	Graphics* graphics;

	Animation	*runningNormalAnimation,
				*runningUpAnimation,
				*runningHittingRightAnimation,
				*rollingAnimation,
				*jumpingAnimation,
				*startingAnimation,
				*acrobatDame;
	bool isFalling;
	bool acrobat;
	bool moveRight;
	bool moveLeft;
	bool jump;
	bool moveHorizontal;
	bool moveVertical;

	bool isCollidingPort;
	bool moveToFontGate;
	float distance;

	BulletPool *bulletPool;
	BoomBombPool* boomPool;
	RocketPool* rocketPool;

	list<CollisionReturn> *listCollide;

	map<int,BaseObject*>* listCanCollide;
	
	map<int, BaseObject*>* listWallCanCollide;
	bool visible;
	//when boom explose in samus, boomVelocity is set based on side of samus and boom
	VECTOR2 boomVelocity;
	bool isBoomExplose;
#pragma region items
	bool isMariMaru;

	int numLives;
	list<Sprite*> *listNumLives;
	int* health;
	Sprite* labelIconHealth;
	Text* textHealth;

	bool isBomb;

	bool isLongBeam;

	int* missitleRocket;
	Sprite* labelIconRoket;
	Text* textNumRocket;

	eSkin currentSkin;

	bool isRocket;
#pragma endregion



	bool canGoElevator;

	bool isHitAlienSmall;

	bool isHitAlienBig;

	float timerInFire;

	bool creamAttack;

	CollideDirection directCollide;
public:
	float timerShoot;

	void setDirectCollide(CollideDirection direct);
	CollideDirection getDirectCollide();

	Samus(TextureManager* textureM, Graphics* graphics, Input* input);
	Samus();
	~Samus();

	bool getRocket();
	void setHitAlienSmall(bool hit);

	void setHitAlienBig(bool hit);

	void handleInput(float dt);
	void onCollision(float dt);

	void update(float dt);
	void draw();
	void drawInFrontMap();
	void release();

	void updateHorizontal(float dt);
	void updateVertical(float dt);
	
	bool isFaling();
	bool isAcrobat();
	bool canMoveLeft();
	bool canMoveRight();
	bool canJump();

	void setCanJump(bool jump);
	void setCanMoveLeft(bool moveLeft);
	void setCanMoveRight(bool moveRight);
	void setFall(bool isFall);
	void setAcrobat(bool acrobat);

	void setBoundCollision(MetroidRect rect);
	void setActiveBound();
	void setVisible(bool visible);
	bool getVisible();

	void setHealth(int heal);
	int* getHealth();

	void setIsCollidingPort(bool flag);
	bool isColliedPort();
	void setCanMoveToFrontGate(bool flag);
	void setCanMoveElevator(bool flag);
	bool getCanMoveElevator();
	void setListCanCollide(map<int,BaseObject*>* list);
	void setListWallCanCollide(map<int, BaseObject*>* list);
	void setBoomVelocity(VECTOR2 boomVelocity);
	VECTOR2 getBoomVelocity();
	void setBoomExplose(bool boomExplose);
	bool getBoomExplose();
	Animation* getStartingAnim();
	Animation* getRunningNormalAnim();
	Animation* getRunningUpAnim();
	Animation* getRunningHittingRightAnim();
	Animation* getRollingAnim();
	Animation* getJumpingAnim();
	Animation* getAcroBatDame();
	list<CollisionReturn> *getListCollide();
	map<int, BaseObject*>* getListCanCollide();
	map<int, BaseObject*>* getListWallCanCollide();

#pragma region items
	bool isHaveMariMaru();
	void setMariMaru(bool flag);

	int getNumLive();
	void setNumLive(int num);

	bool isHaveBomb();
	void setBomb(bool flag);

	bool isHaveLongBeam();
	void setLongBeam(bool flag);

	int* getNumRocket();
	void setNumRocket(int num);
	
	eSkin getCurrentSkin();
	void setNewSkin(eSkin skin);
	void setDataSuiteSkin(int yellow, int yellowIce, int pink, int pinkIce);

	void setCreamAttack(bool creamAttack);
	bool isCreamAttack();
#pragma endregion

	void setTimerInFire(float time);
	float getTimerInFire();

};

