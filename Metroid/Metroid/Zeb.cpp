#include "Zeb.h"

#define VELOCITY_X 120
#define VELOCITY_Y 120
#define TIME_FRAME_DELAY 0.2f


Zeb::Zeb()
{
}

Zeb::Zeb(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::ZEB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zeb");
	}
	this->samusPosition = VECTOR2ZERO;

	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebYellow, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 1;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebBrown, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 1;
		break;
	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebRed, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 2;
		break;

	default:
		break;
	}

	this->setOrigin(VECTOR2(0.5, 0.5));

	this->anim->start();
	isActivity = false;
}


Zeb::~Zeb()
{
	delete this->anim;
}
void Zeb::reInit()
{
	allowFly = false;
	isActivity = false;
	this->setPosition(startPosition);
}
void Zeb::handleVelocity(float dt)
{
	if (isActivity == true)
	{
		this->anim->update(dt);

		if (this->getPosition().y < this->samusPosition.y && allowFly == false)
		{
			this->velocity.y = VELOCITY_Y;
			this->velocity.x = 0;

		}
		if (this->getPosition().y >= this->samusPosition.y && allowFly == false)
		{
			this->velocity.x = VELOCITY_X*direction;
			this->velocity.y = 0;
			allowFly = true;
		}
		
	}
	else
	{
		isActivity = true;
	}

	if (this->getPosition().x < Camera::getInstance()->getBound().left || this->getPosition().x > Camera::getInstance()->getBound().right)
	{		
		reInit();
	}
}

void Zeb::update(float dt)
{
	this->setPosition(VECTOR2(this->getPosition().x + velocity.x*dt,
		this->getPosition().y + velocity.y*dt));
}

void Zeb::draw()
{
	this->sprite->draw();
}

void Zeb::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;

	this->boundCollision = rect;
}


void Zeb::setStartPosition(VECTOR2 startPosition)
{
	this->startPosition = startPosition;
}


void Zeb::setTarget(VECTOR2 samusPosition)
{
	this->samusPosition = samusPosition;
	if (this->getPosition().x - samusPosition.x > 0)
	{
		this->setDirection(eDirection::left);
		this->setScaleX(1);
	}
	else
	{
		this->setDirection(eDirection::right);
		//this->setScaleX(-1);
	}
}


