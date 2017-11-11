#include "SamusStateRunning.h"
#include "SamusStateManager.h"


SamusStateRunning::SamusStateRunning()
{
}

SamusStateRunning::SamusStateRunning(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateRunning::~SamusStateRunning()
{
}

void SamusStateRunning::init()
{
	runningNormal = samus->getRunningNormalAnim();
	runningUp = samus->getRunningUpAnim();
	runningHittingRight = samus->getRunningHittingRightAnim();
	
	this->samus->setOrigin(VECTOR2(0.5f, 1));

	this->animation = runningNormal;
}

void SamusStateRunning::handleInput(float dt)
{
#pragma region Horizontal
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setScaleX(-1);
			this->samus->setDirection(eDirection::left);
		}

		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyUp(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);
			this->samus->setDirection(eDirection::right);
		}

		this->samus->updateHorizontal(dt);
	}

	if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
	{
		// must fix because of origin sprite change
		if (this->samus->isInDirection(eDirection::left))
		{
			if (isUp)
				this->samus->setPositionX(this->samus->getPosition().x + SAMUS_WIDTH_UP_HALF);
			else
				this->samus->setPositionX(this->samus->getPosition().x + SAMUS_WIDTH_RL_HALF);
		}
		else if (this->samus->isInDirection(eDirection::right))
		{
			if (isUp)
				this->samus->setPositionX(this->samus->getPosition().x - SAMUS_WIDTH_UP_HALF);
			else
				this->samus->setPositionX(this->samus->getPosition().x - SAMUS_WIDTH_RL_HALF);
		}

		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}
#pragma endregion



	if (input->isKeyDown(VK_UP))
	{
		this->animation->stop();

		this->animation = runningUp;
		this->animation->start();
	}

	if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
	{
		this->animation->stop();

		this->animation = runningHittingRight;
		this->animation->start();

		this->isUp = false;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyDown(VK_UP) && input->isKeyDown(VK_Z))
	{
		this->animation->stop();

		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		this->samus->setOrigin(VECTOR2(0.5f, 1.0f));

		this->animation = runningUp;
		this->animation->start();

		this->isUp = true;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)))
	{
		this->animation->stop();

		this->animation = runningNormal;
		this->animation->start();
	}

	if (input->isKeyDown(VK_X))
	{
		this->animation->stop();

		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}
}

void SamusStateRunning::update(float dt)
{
	this->animation->update(dt);
}

void SamusStateRunning::onStart()
{
	this->animation->start();
}

void SamusStateRunning::onExit()
{
	this->animation->stop();
}

void SamusStateRunning::fire()
{
	VECTOR2 stP;
	VECTOR2 tar;

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.8f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.8f);

		tar = VECTOR2(stP.x, stP.y - 100);
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.4f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x + 100, stP.y);
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.4f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x - 100, stP.y);
		}
	}

	BulletPool::getInstance()->getBullet()->init(stP, tar);
}
