#include "Rio.h"
#define RATE_BEZIER 0.3f

Rio::Rio(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::RIO)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Rio");
	}

	this->sprite->setOrigin(VECTOR2(0.5f, 0.5f));

	animation = new Animation(this->sprite, IndexManager::getInstance()->rioBlue, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
	animation->start();

	this->startPosition = VECTOR2(200, 100);
	this->target = VECTOR2ZERO;
	this->P1 = this->startPosition;
	this->P3 = VECTOR2(P1.x + WIDTH_AREA_ACTIVE, startPosition.y);
	this->P5 = VECTOR2(P3.x + WIDTH_AREA_ACTIVE, startPosition.y);
	this->setPosition(this->P1);

	t = 0;
	t1 = 0;
}

Rio::Rio()
{
}


Rio::~Rio()
{

}

void Rio::update(float dt)
{
	this->animation->update(dt);
	if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START))
	{
		if (P1.x < target.x && target.x < P3.x && getStatusRollTarget() == true)
		{
			flag = 0;
			this->setStatus(eStatus::FALLING);
		}
		if (P1.x < target.x && target.x < P3.x && getStatusRollTarget() == false)
		{
			this->setStatus(eStatus::FOLLOW);
		}
	}

	if (this->isInStatus(eStatus::FALLING))
	{
		if (flag == 0) // target is the left area
		{
			if (t < 1)
			{
				t += dt * RATE_BEZIER;
				this->P2 = VECTOR2((this->P1.x + this->P3.x) / 2, target.y*1.6f);
				this->setPosition((1 - t)*(1 - t)*P1 + 2 * (1 - t)*t*P2 + t*t*P3);
				if (P3.x < target.x && target.x < P5.x && this->getPosition().x > P3.x - 1) {
					flag = 1;
					t = 0;
				}
				t1 = 0;
			}
			else
			{
				if (t1 < 1) {
					t1 += dt * RATE_BEZIER;
					this->P2 = VECTOR2((this->P3.x + this->P1.x) / 2, target.y*1.6f);
					this->setPosition((1 - t1)*(1 - t1)*P3 + 2 * (1 - t1)*t1*P2 + t1*t1*P1);
				}
				else {
					t = 0;
				}
			}
		}
		if (flag == 1)// target is the right area
		{
			if (t < 1)
			{
				t += dt * RATE_BEZIER;
				this->P4 = VECTOR2((this->P3.x + this->P5.x) / 2, target.y*1.6f);
				this->setPosition((1 - t)*(1 - t)*P3 + 2 * (1 - t)*t*P2 + t*t*P5);
				t1 = 0;
			}
			else
			{
				if (t1 < 1) {
					t1 += dt * RATE_BEZIER;
					this->P2 = VECTOR2((this->P3.x + this->P5.x) / 2, target.y*1.6f);
					this->setPosition((1 - t1)*(1 - t1)*P5 + 2 * (1 - t1)*t1*P2 + t1*t1*P3);
					if (P1.x < target.x && target.x < P3.x && this->getPosition().x < P3.x + 1) {
						flag = 0;
						t1 = 0;
					}
				}
				else {
					t = 0;
				}
			}
		}
		if (getStatusRollTarget() == false) {
			this->setStatus(eStatus::FOLLOW);
		}
	}
	if (this->isInStatus(eStatus::FOLLOW))
	{
		t = 0;
		if (t < 1)
		{
			t += dt * RATE_BEZIER * 3;
			this->setPosition((1 - t)*(1 - t)*this->getPosition() + 2 * (1 - t)*t*VECTOR2(target.x, target.y) + t*t*target);
		}
		else
		{
			this->setPosition(target.x, target.y);
		}
	}
}

void Rio::draw()
{
	this->sprite->draw();
}

void Rio::release()
{
	delete this->sprite;
	delete this->animation;

	this->sprite = nullptr;
	this->animation = nullptr;
}

VECTOR2 Rio::getTarget()
{
	return this->target;
}

bool Rio::getStatusRollTarget() {
	return this->statusRollTarget;
}
void Rio::setTarget(VECTOR2 target, bool statusRollTarget)
{
	this->target = target;
	this->statusRollTarget = statusRollTarget;
}