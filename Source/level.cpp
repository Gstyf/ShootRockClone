#include "level.h"
#include "game.h"
#include "myMath.h"
#include "raymath.h"

void Level::ChargeLaser()
{
	if (IsKeyDown(KEY_Z))
	{
		chargeCounter++;

		// play sounds to audibly indicate when charge begins and when finished.
		if (chargeCounter == 1)
		{
			PlaySoundMulti(Resources::sounds[2]);
		}
		else if (chargeCounter == chargeFull)
		{
			PlaySoundMulti(Resources::sounds[3]);
		}

		// if-statements will make the textures move toward the front and centre visually indicating if charge is complete
		if (chargeCounter < chargeFull)
		{
			if (chargeCounter % 2 == 0)
			{
				DrawTextureV(Resources::laser[1],
					allEntities[0]->position - Vector2{ (50.f - (20 / (chargeFull / chargeCounter))),(50.f + (10 / (chargeFull / chargeCounter))) }, WHITE); //left of ship
				DrawTextureV(Resources::laser[2],
					allEntities[0]->position - Vector2{ (-10.f + (10 / (chargeFull / chargeCounter))) , (50.f + (10 / (chargeFull / chargeCounter))) }, WHITE); //right of ship
			}
			else
			{
				DrawTextureV(Resources::laser[2],
					allEntities[0]->position - Vector2{ (50.f - (20 / (chargeFull / chargeCounter))), (50.f + (10 / (chargeFull / chargeCounter))) }, WHITE); //left of ship
				DrawTextureV(Resources::laser[1],
					allEntities[0]->position - Vector2{ (-10.f + (10 / (chargeFull / chargeCounter))), (50.f + (10 / (chargeFull / chargeCounter))) }, WHITE); //right of ship
			}
		}
		else if (chargeCounter >= chargeFull)
		{
			if (chargeCounter % 2 == 0)
			{
				DrawTextureV(Resources::laser[3], allEntities[0]->position - Vector2{ 30,60 }, WHITE);
				DrawTextureV(Resources::laser[4], allEntities[0]->position - Vector2{ 10,60 }, WHITE);
			}
			else
			{
				DrawTextureV(Resources::laser[4], allEntities[0]->position - Vector2{ 30,60 }, WHITE);
				DrawTextureV(Resources::laser[3], allEntities[0]->position - Vector2{ 10,60 }, WHITE);
			}
		}
	}

	// Laser will only fire if Z-key is held down for 90 frames - or 1.5 seconds
	if (IsKeyReleased(KEY_Z))
	{
		if (chargeCounter >= chargeFull)
		{
			SpawnEntity(EntityKind::PROJECTILE,
				Vector2{ (allEntities[0]->position.x), ((allEntities[0]->position.y) - 10) });
			PlaySoundMulti(Resources::sounds[1]);
		}
		chargeCounter = 0;
	}
}

void Level::AddScore()
{
	score += (50 + (10 * (scoreBonus)));
	scoreBonus++;
	scoreBonusCounter = 0;
}

void Level::AddEntity(Entity temp)
{
	entityList.push_back(temp);
	allEntities.push_back(&entityList.back());
}

void Level::RemoveEntity()
{ // this is almost exactly the removeEntity code for the goat & tree game we used in programming 1, but I changed the names to fit my code. Lazy, I know, but I didn't know of a better way to do it.
	auto dead_entity = std::remove_if(allEntities.begin(), allEntities.end(), [](const Entity* e) -> bool { return e->isDead; });
	allEntities.erase(dead_entity, allEntities.end());
	entityList.remove_if([](const Entity& e) -> bool { return e.isDead; });
}

void Level::SpawnEntity(EntityKind e, Vector2 v)
{
	Entity tEntity;
	tEntity.kind = e;
	tEntity.position = v;
	switch (e)
	{
	case EntityKind::PLAYER:
		tEntity.size = 40;
		tEntity.speed = 8;
		break;
	case EntityKind::PROJECTILE:
		tEntity.size = 6;
		tEntity.speed = 15;
		break;
	case EntityKind::ROCK:
		tEntity.size = 21;
		tEntity.speed = 5;
		tEntity.direction = { (float)GetRandomValue(-1, 1), 1 }; // gives rocks their random side-ways trajectory
		tEntity.texture = GetRandomValue(0, 3); // assigns a random texture to rocks, so they look different
		break;
	case EntityKind::COIN:
		tEntity.size = 15;
		tEntity.speed = 8;
		tEntity.texture = GetRandomValue(0, 2); // assigns a random texture to coins, so the look different
		break;
	case EntityKind::SCORE:
		tEntity.size = 30;  // Used here as a counter in update. Once 0 it will disappear.
		tEntity.speed = 5;
		tEntity.direction = { -1,-1 }; // Score moves diagonally away from player and upwards.
		tEntity.score = 50 + (10 * scoreBonus); // this is the number that will be read out when collecting coins.
		break;
	default:
		break;
	}
	AddEntity(tEntity);
}

void Level::AddFutureEntity(EntityKind e, Vector2 v)
{
	Entity fEntity;
	fEntity.kind = e;
	fEntity.position = v;
	futureEntities.push_back(fEntity);
}

void Level::SpawnFutureEntity()
{
	for (const Entity& e : futureEntities) // retrieves the entities in futureEntities by address instead of copying them over. Saves memory.
	{
		if (e.kind == EntityKind::COIN)
		{
			for (int i = 0; i < 4; i++)
			{
				SpawnEntity(e.kind, (e.position + Vector2{ ((float)GetRandomValue(-75, 75)),((float)GetRandomValue(-75,75)) }));
			}
		}
		else if (e.kind == EntityKind::SCORE)
		{
			SpawnEntity(e.kind, e.position);
		}
	}
	futureEntities.clear();
}

void Level::Reset()
{
	for (Entity* e : allEntities)
	{
		e->isDead = true;
	}
	RemoveEntity();
	score = 0;
}

Vector2 Level::MovePlayer()
{
	//Can be modified fairly easily to allow for y-axis movement.
	//float yMovement = (float)IsKeyDown(KEY_UP) - (float)IsKeyDown(KEY_DOWN);
	//Change the return to return Vector2{ xMovement,yMovement };
	float xMovement = (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT);
	return Vector2{ xMovement,0 };
}

void Level::ResolveCollision()
{
	for (Entity* e : allEntities)
	{
		for (Entity* c : allEntities)
		{
			switch (e->kind) // Sometimes crashes immediately,sometimes plays well for ages...
			{
			case EntityKind::PLAYER:
				if (c->kind == EntityKind::ROCK)
				{
					if (CheckCollisionPointCircle(c->position, e->position, e->size))
					{
						e->isDead = true;
						c->isDead = true;
						AddFutureEntity(EntityKind::COIN, c->position);
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				else if (c->kind == EntityKind::COIN)
				{
					if (CheckCollisionPointCircle(c->position, e->position, e->size))
					{
						c->isDead = true;
						AddScore();
						AddFutureEntity(EntityKind::SCORE, e->position);
						PlaySoundMulti(Resources::sounds[4]);
					}
				}
				break;
			case EntityKind::PROJECTILE:
				if (c->kind == EntityKind::ROCK)
				{
					if (CheckCollisionPointCircle(e->position, c->position, c->size))
					{
						c->isDead = true;
						AddFutureEntity(EntityKind::COIN, c->position);
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

bool Level::CheckOutOfBounds(Vector2 v)
{
	if (v.x < -20 || v.x > GetRenderWidth() + 20 || v.y < -100 || v.y > GetRenderHeight() + 20)
	{
		return true;
	}
	else return false;
}

void Level::Update()
{
	for (Entity* e : allEntities) // This for-each with a nested switch-case moves all objects every frame. I use this loop because I can retrieve EntityKind easier (e->kind, rather than allEntities[e]->kind).
	{
		switch (e->kind)
		{
		case EntityKind::PLAYER:
			e->position += MovePlayer() * e->speed;
			//The following statements just returns the player inside bounds if they try to move out of the screen.
			if (e->position.x < 0) { e->position.x = 0; }
			else if (e->position.x > GetRenderWidth()) { e->position.x = GetRenderWidth(); }
			//if (e->position.y < 0) { e->position.y = 0; }
			//else if (e->position.y > GetRenderHeight()) { e->position.y = GetRenderHeight(); }
			break;
		case EntityKind::PROJECTILE:
			e->position.y -= e->speed;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::ROCK:
			e->position.x += e->direction.x;
			e->position.y += e->direction.y * e->speed;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::COIN:
			e->position.y += e->speed;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::SCORE:
			e->position += e->direction * e->speed;
			e->size--;
			if (e->size <= 0) { e->isDead = true; }
			break;
		default:
			break;
		}
	}

	ResolveCollision();
	RemoveEntity();

	ChargeLaser();

	if (futureEntities.size() != 0)
	{
		SpawnFutureEntity();
	}

	int nextAsteroidIn = 60;
	frameCounter++;
	if (frameCounter == nextAsteroidIn)
	{
		SpawnEntity(EntityKind::ROCK, Vector2{ ((float)GetRandomValue(0, GetRenderWidth())), -20.f });
		frameCounter = 0;
		nextAsteroidIn = GetRandomValue(30, 120);
	}

	scoreBonusCounter++;
	if (scoreBonusCounter > 120)
	{
		scoreBonus = 0;
	}

	Render();
}

void Level::Render()
{
	for (Entity* e : allEntities)
	{
		switch (e->kind)
		{
		case EntityKind::PLAYER:
			DrawTextureV(Resources::player, (e->position - Vector2{ 49,37 }), WHITE);
			//DrawCircle((int)e->position.x, (int)e->position.y, e->size, RAYWHITE);
			break;
		case EntityKind::PROJECTILE:
			DrawTextureV(Resources::laser[0], (e->position - Vector2{ 6, 0 }), WHITE);
			if (CheckOutOfBounds(e->position)) {}
			break;
		case EntityKind::ROCK:
			DrawTextureV(Resources::asteroids[e->texture], (e->position - Vector2{ 21,21 }), WHITE);
			break;
		case EntityKind::COIN:
			DrawTextureV(Resources::coins[e->texture], (e->position - Vector2{ 15,15 }), WHITE);
			break;
		case EntityKind::SCORE:
			DrawText(TextFormat("%i", e->score), e->position.x, e->position.y, 20, RAYWHITE);
			break;
		default:
			break;
		}
	}
	DrawText(TextFormat("Score: %i", score), ((GetRenderWidth() / 5) * 2), 20, 20, LIGHTGRAY);
}



