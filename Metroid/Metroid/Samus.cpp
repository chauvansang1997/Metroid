#include "Samus.h"
#include "SamusStateManager.h"

Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite character");
	}

	this->sprite->setPosition(GVector2(0, GAME_HEIGHT*0.8));

	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);
	runningNormalAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningUpAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningUpRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningShootAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowHittingAndRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.07f);
	jumpingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowJumpingRight, NUM_FRAMES_SAMUS_JUMPING, 0.07f);

	SamusStateManager::getInstance()->init(this, input);
	this->isFalling = false;
	this->totalHeightWasJumped = false;
}

Samus::Samus()
{
	this->sprite = new Sprite();
}

Samus::~Samus()
{
	this->release();
}

void Samus::draw()
{
	this->sprite->draw();
}

void Samus::handleInput(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);
}


void Samus::update(float dt)
{
	updateDirection();
	SamusStateManager::getInstance()->getCurrentState()->update(dt);
}

void Samus::release()
{
	input = nullptr;
	sprite = nullptr;
	runningNormalAnimation = nullptr;
	runningUpAnimation = nullptr;
	runningShootAnimation = nullptr;
	rollingAnimation = nullptr;
	startingAnimation = nullptr;
	jumpingAnimation = nullptr;

	delete input, sprite, runningNormalAnimation, runningUpAnimation, runningShootAnimation, rollingAnimation, startingAnimation, jumpingAnimation;
}

void Samus::updateDirection()
{
	if (input->isKeyDown(VK_RIGHT))
	{
		this->setDirection(eDirection::right);
	}

	if (input->isKeyDown(VK_LEFT))
	{
		this->setDirection(eDirection::left);
	}
}

void Samus::updateHorizontal(float dt)
{
	this->setPosition(this->getPositionX() + SAMUS_VERLOCITY_X*dt*getDirection(), this->getPositionY());
}

void Samus::updateVertical(float dt)
{
	if (this->isFalling)
	{
		this->setPosition(this->getPositionX(), this->getPositionY() + SAMUS_VERLOCITY_Y*dt);
	}
	else
	{
		float hps = SAMUS_VERLOCITY_Y*dt*(-1);
		totalHeightWasJumped += hps;

		if (totalHeightWasJumped <= MAX_HEIGHT_CAN_JUMP)
			this->setPosition(this->getPositionX(), this->getPositionY() + SAMUS_VERLOCITY_Y*dt*(-1));
		else
			this->setFall(true);
	}
}

bool Samus::isFaling()
{
	return this->isFalling;
}

void Samus::setFall(bool isFall)
{
	this->isFalling = isFall;
}

Animation * Samus::getStartingAnim()
{
	return this->startingAnimation;
}

Animation * Samus::getRunningNormalAnim()
{
	return this->runningNormalAnimation;
}

Animation * Samus::getRunningUpAnim()
{
	return runningUpAnimation;
}

Animation * Samus::getRunningShootAnim()
{
	return runningShootAnimation;
}

Animation * Samus::getRollingAnim()
{
	return this->rollingAnimation;
}

Animation * Samus::getJumpingAnim()
{
	return this->jumpingAnimation;
}
