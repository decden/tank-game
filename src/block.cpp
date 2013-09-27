#include "block.h"

#include <cassert>
#include <iostream>
#include <GL/gl.h>

#include "tank_game.h"
#include "graphics_utils.h"

Block::Block(TankGame *game, unsigned int color, std::vector<b2Vec2> vertices)
	: game(game), body(nullptr), color(color), vertices(vertices)
{
	assert(vertices.size() > 2);
}

Block::~Block()
{
	assert(body == nullptr);
}


/*
 * IGameObject elements
 */

GameObjectType Block::getObjectType() const
{
	return GameObject_FixedBlock;
}

bool Block::isAlive() const {
	return true;
}

void Block::draw(float z)
{
	GraphicsUtils::setColor(this->color);
	glBegin(GL_TRIANGLE_FAN);
	for (auto &v : vertices)
	{
		glVertex3f(v.x, v.y, z);
	}
	glEnd();
}

void Block::update(float time) { }


/*
 * IPhysicsGameObject elements
 */

void Block::registerPhysicsObject(b2World *world)
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(0.0f, 0.0f);

	b2PolygonShape shape;
	shape.Set(&vertices[0], vertices.size());

	body = world->CreateBody(&bd);
	body->CreateFixture(&shape, 1.0f);
	body->SetUserData(this);
}

void Block::unregisterPhysicsObject(b2World *world)
{
	if (body)
	{
		world->DestroyBody(body);
		body = nullptr;
	}
}

void Block::beforePhysicsStep() { }
void Block::afterPhysicsStep() { }

void Block::handleCollision(IPhysicsGameObject *other)
{
	// FIXME implement this
}

std::shared_ptr<Block> Block::createRectangle(TankGame *game, unsigned int color, float x, float y, float w, float h)
{
	std::vector<b2Vec2> vertices(4);
	vertices[0].Set(x,   y);
	vertices[1].Set(x+w, y);
	vertices[2].Set(x+w, y+h);
	vertices[3].Set(x,   y+h);
	auto block = std::make_shared<Block>(game, color, vertices);
	return block;
}