# ShootRockClone
A small school assignment project to clone a 2D arcade game. In this case a space shooter. The assignment also required the use of as little code as possible.

As the title suggests, it is a clone of a game called 'Shoot Rock', found at this url: https://krajzeg.itch.io/shoot-rock

This project was made at the end of my first semester of my Game Design and Programming course's first semester and is written entirely in C++ using the Raylib library. In some respects it is more primitive than Shoot Rock since movement is entirely locked to the x-axis, which was part of the assignment design brief. It is also written using only around 600 lines, which is something I might try to revisit in the future to push down even further. I do not know how that compares to the original. 

The game iself it fairly basic. There is no storage of high scores, so if you die you will lose the score forever. This is something I can easily fix. I also took a short-cut with the entity spawning; it works, but I can make it look a lot nicer.

A lot of the challenge of this project was figuring out how I can reuse existing methods to cut down on redundant repetition of code, something I know I can do even better today a year later. I would also change the code of movement and spawning to make use to interpolation, inertia and acceleration to give it a more satisfying game feel. I would also like to change it to have more defined 'levels' for difficulty curves.