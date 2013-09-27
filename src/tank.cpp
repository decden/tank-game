#include "tank.h"

#include <cassert>
#include <memory>
#include <cmath>
#include <iostream>
#include <GL/gl.h>

#include "graphics_utils.h"
#include "math_utils.h"
#include "tank_game.h"

Tank::Tank(TankGame *game, TankType type, float x, float y)
	: game(game), body(nullptr), 
	type(type), alive(true), x(x), y(y),
	angle(0.0f), turret_angle(0.0f)
{
	// Set the tank shape (as used by physics engine)
	shape_vertices[0].Set(-0.6, -1.0);
	shape_vertices[1].Set( 0.6, -1.0);
	shape_vertices[2].Set( 1.0, -0.6);
	shape_vertices[3].Set( 1.0,  0.6);
	shape_vertices[4].Set( 0.6,  1.0);
	shape_vertices[5].Set(-0.6,  1.0);
	shape_vertices[6].Set(-1.0,  0.6);
	shape_vertices[7].Set(-1.0, -0.6);

	// Set the colors, based on type
	switch (type)
	{
		case Tank_Player:
			body_color = 0x006680ff; wheels_color = 0x00aad4ff; turret_color = 0x00161cff;
			break;
		case Tank_EnemySimple:
		default:
			body_color = 0x800033ff; wheels_color = 0xd40055ff; turret_color = 0x1c000bff;
	}
}

Tank::~Tank() { }

/*
 * IGameObject Functions
 */

GameObjectType Tank::getObjectType() const
{
	return GameObject_Tank;
}

bool Tank::isAlive() const
{
	return alive;
}

void Tank::draw(float z)
{
	glPushMatrix();

	// Move and rotate the tank to the right position
	glTranslatef(this->x, this->y, z);
	glRotatef(rad2deg(this->angle), 0.0f, 0.0f, 1.0f);

	// Draw the wheels and the body
	GraphicsUtils::setColor(this->wheels_color);
	GraphicsUtils::fillRectangle(-0.9f, -1.0f, 1.8f, 2.0f, 0.0f);
	GraphicsUtils::setColor(this->body_color);
	GraphicsUtils::fillRectangle(-1.0f, -0.6f, 2.0f, 1.2f, 0.1f);

	glPopMatrix();
	glPushMatrix();

	// Move and rotate the modelview matrix to the right position
	glTranslatef(this->x, this->y, z);
	glRotatef(rad2deg(this->turret_angle), 0.0f, 0.0f , 1.0f);

	// Draw the turret middle circle
	GraphicsUtils::setColor(this->turret_color);
	GraphicsUtils::fillCircle(0, 0, 0.4, 6, 25);

	GraphicsUtils::fillRectangle(0.0f, -0.1f, 0.8f, 0.2f, 0.5f);
	GraphicsUtils::fillRectangle(0.8f, -0.2f, 0.2f, 0.4f, 0.5f);

	glPopMatrix();
}

void Tank::update(float) { }


/*
 * IPhysicsGameObject Functions
 */

void Tank::registerPhysicsObject(b2World *world)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.angle = angle;
	bd.position.Set(x, y);

	// The Shape for a tank, is kind of an octagon
	b2PolygonShape shape;
	shape.Set(shape_vertices, 8);

	body = game->getPhysicsWorld()->CreateBody(&bd);
	body->CreateFixture(&shape, 1.0f);

	body->SetAngularVelocity(0.2f);
	body->SetLinearVelocity(b2Vec2 { 0.5f, 0.8f });
	body->SetLinearDamping(0.2f);
	body->SetAngularDamping(0.2f);
	body->SetUserData(this);
}

void Tank::unregisterPhysicsObject(b2World *world)
{
	if (body)
	{
		game->getPhysicsWorld()->DestroyBody(body);
		body = nullptr;
	}
}

void Tank::beforePhysicsStep()
{

}

void Tank::afterPhysicsStep()
{
	angle = body->GetAngle();

	auto position = body->GetPosition();
	x = position.x;
	y = position.y;
}

void Tank::handleCollision(IPhysicsGameObject *other)
{
	if (other->getObjectType() == GameObject_Bullet)
	{
		Bullet *bullet = dynamic_cast<Bullet*>(other);
		assert(bullet);
		if (bullet->canDamnageTank(this))
		{
			this->alive = false;
			// FIXME: Notify game
		}
	}
}



void Tank::turnBody(float angle)
{
	this->angle += angle;
}

void Tank::turnTurret(float angle)
{
	this->turret_angle += angle;
}

void Tank::fireBullet(float speed)
{
	// If the tank is not alive, don't shoot anything!
	if (!this->alive) return;

	float x = this->x + std::cos(turret_angle) * 1.0f;
	float y = this->y + std::sin(turret_angle) * 1.0f;

	auto bullet = std::make_shared<Bullet>(this, x, y, turret_angle, speed);
	game->addBullet(bullet);

	// Apply some force in the opposite direction!
	if (body)
	{
		body->ApplyForceToCenter(b2Vec2 { -std::cos(turret_angle)*10.0f, -std::sin(turret_angle)*10.0f });
	}
}