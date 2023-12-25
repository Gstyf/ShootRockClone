#pragma once
#include "entity.h"
#include <list>

// This header file defines the struct Level. Level stores all entities & handles logic by updating Entity position, adding, and removing entites where appropriate and checking for collision and whether entities are still on screen.

struct Level
{
	std::list<Entity> entityList = {};		// a list containing all the entities. I'm using a list because I will be adding and removing entities CONSTANTLY.
	std::vector<Entity*> allEntities = {};	// a vector containing pointers to the address in memory for all entities. I use the vector because I always want to be able to retrieve index[0] since that's the player.

	/* These fields are used for the asteroid spawning & laser charging */
	int frameCounter	= 0;	// this will be used to time when asteroid should be spawned.
	int chargeCounter	= 0;	// this will be used to see how long the laser's been charged for.
	int chargeFull		= 90;	// how many frames you need to charge for to fire. I am declaring this here so I can update it only once if I need to finetune it.
	void ChargeLaser();			// charges laser, fires if charged for long enough, or doesn't if not.

	/* These fields are used to track the score system, and the bonus points for collecting coinds in rapid succession */
	int score = 0;			// tracks score... pretty self explanatory. Increments when the AddScore() function is called.
	int scoreBonus = 0;		// when coins are picked up consecutively in short order the amount of point given is multiplied by this amount.
	int scoreBonusCounter = 0; // a timer that tracks frames since last coin was picked up. After enough frames have based, bonus is reset to 0.
	void AddScore();		// Will add score taking in the base score for each coin and add a bonus for consecutive coins. Sets the scoreBonusCounter to 0 when called.

	void AddEntity(Entity temp);	// adds entities to the above list and vector of pointers
	void RemoveEntity();			// goes through the entities and removes the ones that return true on the boolean isDead.
	void SpawnEntity(EntityKind e, Vector2 v); // takes an EntityKind and a Vector2 as an argument. Using a switch-case it pushes the corresponding entity using AddEntity().

	/* This vector and methods handles queueing entities to be spawned at beginning of next update after ResolveCollision() flags Coins and Rocks as dead */
	/* This is to prevent the vector allEntities resizing during Update() which would occasionally cause a crash */
	std::vector<Entity> futureEntities = {};		// stores temporary entities when Rocks are destroyed or coins are collected.
	void AddFutureEntity(EntityKind e, Vector2 v);	// takes EntityKind and Vector2 as argument. Creates temporary entity and pushes to futureEntites.
	void SpawnFutureEntity();						// calls SpawnEntity() for each entity in futureEntities.

	void Reset();	// sets every entity to dead and removes them using RemoveEnitity
	
	Vector2 MovePlayer(); // returns a Vector2 value which is added/deducted from the player's vector2 position. I return Vector2 in case i want to reinstate vertical movement.


	/* These two methods determine when entities are dead so they can be removed from the game. Called last to prevent allEntities from resizing during iteration. */
	void ResolveCollision();			// uses a switch-case to determine outcome of two entities colliding
	bool CheckOutOfBounds(Vector2 v);	// returns a true/false on whether object is still on screen - sets entity isDead to true if out of bounds.

	void Update();	// for each frame updates all entities' position, checks for collisions, checks if entities are out of bounds, removes dead entities and then renders all entities.
	void Render();	// goes through all entities and draws them according to their position and textures as well as displaying score.

};