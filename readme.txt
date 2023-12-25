		Shoot Rock clone by Gustaf Styf

Texture & sound assets credited to Kenney.nl/assets

	-	-	-	-	-

Introduction:
Welcome to Gurra's Shoot Rock Clone! the goal is to shoot rocks and collect coins while not colliding with said rocks.

Controls:
- Left and Right keys to move left and right. The Assignment Game Description said the space ship only moved hotizontally, so I removed vertical movement. :(

- Z key charges you laser gun. It takes about 1,5 seconds to fully charge. Release Z to fire when fully charged. There are sound cues to help you hear when it's ready. You can also see it visually on the ship as the sparkly bits will flicked at the nose instead of over the wings.

- X key to start the game from main menu.

- Q key to return to the main menu from game.

	-	-	-	-	-

Documentation (in Source Folder):
//.H files
entity.h : Contains the class Entity and defines it.

game.h : Contains the GameState, Resources and GameMaster structs. 

level.h : Contains the Level struct and defines its data structure for entities and methods.

myMath.h : Contains a set of operators to make doing math with Vector2s easier. I did not write this one - it came from Filippo Crocchini.

//.CPP files
game.cpp : Contains a method for loading and unloading resources (textures and sounds) into the Resource struct and methods for starting and ending the game by changing GameState.

level.cpp : Contains all the logic for the game; movement, collision, spawning and deleting entities as well as rendering what is currently on screen in the level.

main.cpp : Initializes the game window, the audio device and the calls the LoadResource method. Also reads up the gameplay controls. GameState switching is also handled here as well as unloading resourses.
