#pragma once
#include <iostream>
#include <vector>
#include "Bullet.h"

class BulletPool
{
private:
	TextureManager* textureManager;
	Graphics* graphics;

	std::vector<Bullet*> list;
	std::vector<Bullet*> listUsing;

	static BulletPool* instance;
public:
	static BulletPool* getInstance();

	BulletPool(TextureManager* textureM, Graphics* graphics);
	BulletPool(TextureManager* textureM, Graphics* graphics, int startNumberElement);
	~BulletPool();

	Bullet* getBullet();
	void returnPool(Bullet* bullet);

	std::vector<Bullet*> getListUsing();
};
