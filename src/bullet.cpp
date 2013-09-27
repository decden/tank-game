#include "bullet.h"

#include <cmath>
#include <cassert>
#include <iostream>
#include <GL/gl.h>

#include "tank_game.h"
#include "graphics_utils.h"
#include "math_utils.h"

Bullet::Bullet(Tank *owner, float x, float y, float direction, float speed)
	: body(nullptr), owner(owner), alive(true), age(0.0f),
	x(x), y(y),
	direction(direction), speed(speed),
	bounces(0)
{ }

Bullet::~Bullet()
{
	assert(body == nullptr);
}


/*
 * IGameObject Functions
 */

GameObjectType Bullet::getObjectType() const
{
	return GameObject_Bullet;
}

bool Bullet::isAlive() const {
	return alive;
}

void Bullet::draw(float z)
{
	glPushMatrix();
	glTranslatef(this->x, this->y, z);
	glRotatef(rad2deg(this->direction), 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLE_FAN);
		// Change colors based on bounces
		// GraphicsUtils::setColor(0x55000000 * (this->bounces) | 0xff);
		GraphicsUtils::setColor(0xff00 << (8*this->bounces) | 0xff);
		glVertex3f(-0.1f, -0.1f, 0.0f);
		glVertex3f( 0.2f, -0.1f, 0.0f);
		glVertex3f( 0.3f,  0.0f, 0.0f);
		glVertex3f( 0.2f,  0.1f, 0.0f);
		glVertex3f(-0.1f,  0.1f, 0.0f);
	glEnd();

	glPopMatrix();
}

void Bullet::update(float time)
{
	this->age += time;
}


/*
 * IPhysicsGameObject Functions
 */

void Bullet::registerPhysicsObject(b2World *world)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x, y);

	b2CircleShape shape;
	shape.m_p.Set(0.0f, 0.0f);
	shape.m_radius = 0.1f;

	body = world->CreateBody(&bd);
	body->CreateFixture(&shape, 1.0f)->SetRestitution(1.0f);

	float v_x = std::cos(direction) * speed;
	float v_y = std::sin(direction) * speed;

	body->SetLinearVelocity(b2Vec2 {v_x, v_y});
	body->SetUserData(this);
	body->SetUserData(this);
}

void Bullet::unregisterPhysicsObject(b2World *world)
{
	if (body)
	{
		world->DestroyBody(body);
		body = nullptr;
	}
}

void Bullet::beforePhysicsStep() { }

void Bullet::afterPhysicsStep()
{
	// Update position
	auto position = body->GetPosition();
	this->x = position.x;
	this->y = position.y;

	// Update direction
	auto velocity = body->GetLinearVelocity();
	this->direction = std::atan2(velocity.y, velocity.x);
}

void Bullet::handleCollision(IPhysicsGameObject *other)
{
	// Kill bullet if it hits another bullet
	if (other->getObjectType() == GameObject_Bullet)
	{
		this->alive = false;
		// FIXME: notify game to display an explosion or/and play a sound
	}
	else if (other->getObjectType() == GameObject_Tank)
	{
		// Note the firing tank has immunity from this bullet the first 400ms
		Tank *tank = dynamic_cast<Tank*>(other);
		if (this->canDamnageTank(tank))
		{
			this->alive = false;
			// FIXME: The other tank should be damnaged
		}
	}
	else
	{
		this->bounces++;
		if (this->bounces >= 3) {
			this->alive = false;
		}
	}

	// Make sure that the velocity remains constant
	auto velocity = body->GetLinearVelocity();
	velocity.Normalize();
	velocity *= speed;
	body->SetLinearVelocity(velocity);
}


bool Bullet::canDamnageTank(const Tank *tank) const
{
	return (tank != owner || age > 0.4f);
}