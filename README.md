# HelicopterGame
**Roșu Mihai Cosmin**

Project made for the Introduction to Graphics Programming course of the Graphics, Multimedia and Virtual Reality master program.

## Details

Small OpenGL game where the player can move around a helicopter by clicking on the terrain.

- The terrain is created using noise inside the vertex and fragment shaders
- The trees are created by recursively rendering the branches
- In order to click on the terrain, the program uses a secondary color buffer to store the position of the cursor

## How to run
In order to run this game you need the following framework: https://github.com/UPB-Graphics/gfx-framework-ipg

Then, you have to copy the **tema** directory from this repository inside the **lab** directory of the framework.

Lastly, you have to add
```cpp
#include "lab/tema/tema.h"
```
to the lab_list.h file and
```cpp
World* world = new lab::Tema();
```
to the main.cpp.
