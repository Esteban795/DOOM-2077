# Doom2077

Doom2077 is a multiplayer video game that is inspired by the principles of the original Doom, released in 1993. The game features retro graphics similar to Doom, but with modern gameplay mechanics.

In Doom2077, the goal is simple: kill as many enemy players as possible. The game is played online, with 2 to 4 players. The single player mode allows you to discover the map.

The game features mechanics from other popular FPS (first-person shooter) games, such as a killfeed (which displays killed players in real time) and the respawn principle (which allows players to reappear on the map after being killed). You can also communicate with other players using the built-in chat.

Doom2077 is a fast-paced and intense game that requires quick reflexes and good strategy to defeat your opponents. There are many weapons with various level of damage and usability, it's up to you to find them and use them effectively.

## How to build and play?

### 1. Clone

First clone the repository, you can do so with the following command:

`git clone --recurse-submodules git@gitlab.telecomnancy.univ-lorraine.fr:ppii-2k24/video-game-02.git`

### 2. Install the dependencies

This project uses as its core the library SDL2 and these particular modules that can be or not packaged separately depending on the Linux distribution you use.

* SDL2
* SDL2_net
* SDL2_ttf 
* SDL2_image 
* SDL2_mixer

One additional library is required: 

* libjansson

### 3. Build the binaries

`make clean all`

### 4. Enjoy!

* Launch the client using `./build/client` (on Linux).

* Launch the server using `./build/server 9999` (on Linux).

### 5. You want more?

* You can generate the doc using `make doc`, and open it with `make open_doc` (at least on Linux).

* You can run the test suite with `make test`