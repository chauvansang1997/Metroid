#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "GameError.h"
#include "BaseObject.h"
#include "Camera.h"
#include "GameDebug.h"
#include "Collision.h"
#include <map>
#include "IFreezable.h"
#include <list>

class Zeb : public BaseObject
{
private:
	Animation *anim;
	VECTOR2 samusPosition;
	VECTOR2 startPosition;
	int health;
	bool flag;

public:
	Zeb();
	Zeb(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Zeb();

	void update(float dt);
	void draw();
	void setBoundCollision();
	void setStartPosition(VECTOR2 startPosition);
	void init(VECTOR2 samusPosition);
};

