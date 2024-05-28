Doom2077                         {#mainpage}
========

# Introduction

Doom2077 is a multiplayer video game that is inspired by the principles of the original Doom, released in 1993. The game features retro graphics similar to Doom, but with modern gameplay mechanics.

In Doom2077, the goal is simple: kill as many enemy players as possible. The game is played online, with 2 to 4 players. The single player mode allows you to discover the map.

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

The meetings and reports can be founds at [this page](cr.html).

## How to build and play?

### 1. Clone

First clone the repository, you can do so with the following command:

`git clone --recurse-submodules git@gitlab.telecomnancy.univ-lorraine.fr:ppii-2k24/video-game-02.git`

\note This repository uses Git submodules, especially for generating the docs, therefore you must use `--recurse-submodules` to
clone the submodules as well.

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

## Compatibility

This game has been developed and tested on the Linux platform under various distributions such as Linux Mint, Arch Linux and NixOS*.

We certify that it runs smoothly on the following platforms: 
* `Linux nemesis 6.8.9-arch1-2 #1 SMP PREEMPT_DYNAMIC Tue, 07 May 2024 21:35:54 +0000 x86_64 GNU/Linux`, `gcc version 14.1.1 20240507`

Some platforms are also known to be incompatible:
* EndeavourOS (x86_64), updated 05/27/2024 in a **VirtualBox** environment, it is suspected that the problem is caused by VirtualBox.

*Under NixOS, it is important to deactivate fsanitize, which is not compatible with the system. 
