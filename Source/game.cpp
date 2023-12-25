#include "game.h"

Texture2D Resources::player;
std::vector <Texture2D> Resources::laser;
std::vector <Texture2D> Resources::asteroids;
std::vector <Texture2D> Resources::coins;

std::vector <Sound> Resources::sounds;

void Resources::LoadResources() 
{
	player = LoadTexture("./Media/playerShip1_red.png");

	laser.push_back(LoadTexture("./Media/laserRed02.png"));
	laser.push_back(LoadTexture("./Media/laserRed09.png"));
	laser.push_back(LoadTexture("./Media/laserRed11.png"));
	laser.push_back(LoadTexture("./Media/laserRed08.png"));
	laser.push_back(LoadTexture("./Media/laserRed10.png"));

	asteroids.push_back(LoadTexture("./Media/meteorBrown_med1.png"));
	asteroids.push_back(LoadTexture("./Media/meteorBrown_med3.png"));
	asteroids.push_back(LoadTexture("./Media/meteorGrey_med1.png"));
	asteroids.push_back(LoadTexture("./Media/meteorGrey_med2.png"));

	coins.push_back(LoadTexture("./Media/star_bronze.png"));
	coins.push_back(LoadTexture("./Media/star_gold.png"));
	coins.push_back(LoadTexture("./Media/star_silver.png"));

	sounds.push_back(LoadSound("./Media/explosionCrunch_000.ogg")); 
	sounds.push_back(LoadSound("./Media/laserLarge_000.ogg"));
	sounds.push_back(LoadSound("./Media/forceField_000.ogg"));
	sounds.push_back(LoadSound("./Media/forceField_002.ogg"));
	sounds.push_back(LoadSound("./Media/impactMetal_000.ogg"));
	sounds.push_back(LoadSound("./Media/laserRetro_001.ogg"));
	sounds.push_back(LoadSound("./Media/explosionCrunch_002.ogg"));
}

void Resources::UnloadResource()
{
	UnloadTexture(player);

	for (int i = 0; i < laser.size(); i++)
	{
		UnloadTexture(laser[i]);
	}

	for (int i = 0; i < asteroids.size(); i++)
	{
		UnloadTexture(asteroids[i]);
	}

	for (int i = 0; i < sounds.size(); i++)
	{
		StopSoundMulti();
		UnloadSound(sounds[i]);
	}
}

void GameMaster::StartLevel()
{
	PlaySoundMulti(Resources::sounds[5]);
	gameState = GameState::GAMEPLAY;
	level.SpawnEntity(EntityKind::PLAYER, Vector2{ ((float)GetRenderWidth() / 2), (((float)GetRenderHeight() / 5) * 4) });
}

void GameMaster::EndLevel()
{
	PlaySoundMulti(Resources::sounds[6]);
	level.Reset();
	gameState = GameState::TITLESCREEN;
}
