Doom2077                         {#mainpage}
========

# Introduction

Doom2077 is a multiplayer video game that is inspired by the principles of the original Doom, released in 1993. The game features retro graphics similar to Doom, but with modern gameplay mechanics.

In Doom2077, the goal is simple: kill as many enemy players as possible. The game is played online, with 2 to 8 players. The single player mode allows you to discover the different maps.

The game features mechanics from other popular FPS (first-person shooter) games, such as a killfeed (which displays killed players in real time) and the respawn principle (which allows players to reappear on the map after being killed). You can also communicate with other players using the built-in chat.

Doom2077 is a fast-paced and intense game that requires quick reflexes and good strategy to defeat your opponents. There are many weapons with various level of damage and usability, it's up to you to find them and use them effectively.

## Project structure

The project is built in a unique repository and shared codebase. Multiple modules exists to strongly seperate the game mechanisms from some utility code.

As such, 5 targets exists: 
* [libnet](dir_2fa1ed3146d6c2b96c5cfe4341985326.html): the network part of the game
* [libcollection](dir_f80612f0d27fb620e0df519c4b358f30.html): regrouping every collections used by the game
* [libecs](dir_03b8e24d0c8528d6b83b13523eccafd1.html): the Entity-Component-System 
* client: the Doom2077 client
* server: the Doom2077 server

Additional information can be found in [specs/](pages.html), such as:

- \ref event
- \ref network