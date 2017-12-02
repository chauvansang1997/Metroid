#include "Samus.h"
#include "SamusStateManager.h"
#include "Camera.h"

#define DISTANCE_MOVE_FRONT_GATE 20

Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Samus");
	}

	this->setPosition(VECTOR2(640, 3312));
	runningNormalAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	runningUpAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningUpRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	runningHittingRightAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowHittingAndRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.05f);
	jumpingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowJumpingRight, NUM_FRAMES_SAMUS_JUMPING, 0.04f);
	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);

	SamusStateManager::getInstance()->init(this, input);

	this->isFalling = false;
	this->acrobat = false;
	this->moveHorizontal = true;
	this->totalHeightWasJumped = 0;
	this->moveLeft = true;
	this->moveRight = true;
	this->jump = true;
	this->distance = 0;

	this->timerShoot = 0;
	bulletPool = new BulletPool(textureM, graphics, 20);
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
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->draw();

	this->sprite->draw();
}

void Samus::handleInput(float dt)
{
	if (!Camera::getInstance()->onPort())
		SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);

#pragma region handle camera
	if (!Camera::getInstance()->onPort() && Camera::getInstance()->getNumPort() < 2)
	{
		if (!Camera::getInstance()->canFolowVertical())
		{
			if ((input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)) || this->isInStatus(eStatus::STANDING))
				Camera::getInstance()->setVelocity(VECTOR2(0, 0));
		}
	}
	Camera::getInstance()->setNumPort(0);
#pragma endregion
}

void Samus::onCollision(BaseObject* object, CollisionReturn result)
{
	SamusStateManager::getInstance()->getCurrentState()->onCollision(object, result);
}

void Samus::setIsCollidingPort(bool flag)
{
	this->isCollidingPort = flag;
}

bool Samus::isColliedPort()
{
	return isCollidingPort;
}

void Samus::setCanMoveToFrontGate(bool flag)
{
	this->moveToFontGate = flag;
}

void Samus::update(float dt)
{

#pragma region handle camera
	if (!Camera::getInstance()->onPort() && Camera::getInstance()->getNumPort() < 2)
	{
		if (!Camera::getInstance()->canFolowVertical())
		{
			if (Camera::getInstance()->canFolowOnLeft())
			{
				if (this->getPosition().x < Camera::getInstance()->getActiveArea().left)
				{
					Camera::getInstance()->setVelocity(VECTOR2(this->getVelocity().x, 0));
				}
			}

			if (Camera::getInstance()->canFolowOnRight())
			{
				if (this->getPosition().x > Camera::getInstance()->getActiveArea().right)
				{
					Camera::getInstance()->setVelocity(VECTOR2(this->getVelocity().x, 0));
				}
			}
		}
		else
		{
			if ((this->getPosition().y > Camera::getInstance()->getActiveArea().top) ||
				(this->getPosition().y < Camera::getInstance()->getActiveArea().bottom))
				Camera::getInstance()->setVelocity(VECTOR2(0, this->getVelocity().y));
		}
	}
#pragma endregion

	if (isCollidingPort)
		this->setVelocityX(Camera::getInstance()->getVelocity().x);

	if (moveToFontGate && !Camera::getInstance()->onPort())
	{
		float dis = dt * SAMUS_VERLOCITY_X;
		this->distance += dis;
		
		if (this->distance < DISTANCE_MOVE_FRONT_GATE)
			this->setPositionX(this->getPosition().x + dis*this->getDirection());
		else
		{
			this->distance = 0;
			moveToFontGate = false;
			this->setStatus(eStatus::STANDING);
		}
	}

	SamusStateManager::getInstance()->getCurrentState()->update(dt);

	this->timerShoot += dt;
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->update(dt);
}

void Samus::release()
{
	delete this->sprite;
	delete runningNormalAnimation;
	delete runningUpAnimation;
	delete runningHittingRightAnimation;
	delete rollingAnimation;
	delete startingAnimation;
	delete jumpingAnimation;
	delete bulletPool;
}

void Samus::updateHorizontal(float dt)
{
	this->setPosition(this->getPosition().x + velocity.x*dt, this->getPosition().y);
}

void Samus::updateVertical(float dt)
{
	this->setPosition(this->getPosition().x, this->getPosition().y + velocity.y*dt);
}

bool Samus::isFaling()
{
	return this->isFalling;
}

bool Samus::isAcrobat()
{
	return acrobat;
}

bool Samus::canMoveLeft()
{
	return this->moveLeft;
}

bool Samus::canMoveRight()
{
	return this->moveRight;
}

bool Samus::isJump()
{
	return this->jump;
}

void Samus::setJump(bool jump)
{
	this->jump = jump;
}

void Samus::setCanMoveLeft(bool moveLeft)
{
	this->moveLeft = moveLeft;
}

void Samus::setCanMoveRight(bool moveRight)
{
	this->moveRight = moveRight;
}

void Samus::setFall(bool isFall)
{
	if (this->isFalling != isFall)
	{
		this->isFalling = isFall;
		
		if (this->isFalling == false)
			this->totalHeightWasJumped = 0;
	}
}
void Samus::setBoundCollision(MetroidRect rect)
{
	this->boundCollision = rect;
	this->setActiveBound();
}

void Samus::setActiveBound()
{
	// Can 1 con so hop ly
	//this->activeBound.top = this->boundCollision.top - 60;
	//this->activeBound.left = this->boundCollision.left - 60;
	//this->activeBound.right = this->boundCollision.right + 80;
	//this->activeBound.bottom = this->boundCollision.bottom + 80;
	RECT r = Camera::getInstance()->getBound();

	this->activeBound = MetroidRect((float)r.top, (float)r.bottom, (float)r.left, (float)r.right);
}

void Samus::setAcrobat(bool acrobat)
{
	this->acrobat = acrobat;
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

Animation * Samus::getRunningHittingRightAnim()
{
	return runningHittingRightAnimation;
}

Animation * Samus::getRollingAnim()
{
	return this->rollingAnimation;
}

Animation * Samus::getJumpingAnim()
{
	return this->jumpingAnimation;
}

