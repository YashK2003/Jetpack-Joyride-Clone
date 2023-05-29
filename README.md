# Jetpack Joyride Clone

A Minimalist clone of the popular Jetpack Joyride game written with OpenGL and C++

## Screenshots of the game

## How To Run
```bash
mkdir libraries 
Add the following three libraries in the above folder
- glad
- glfw
- glm

then do the following

mkdir build
cd build
cmake ..
make
./app
```


## Controls
- `Space` to propel upwards.

## Obstacles and points
- **Zappers**: Zappers are ”lines” of electricity that kill your player on contact. They may be static or moving.

- **Coins**: Collect coins on contact. Determines your score at the end of the game.

## Features
- **Score and Distance covered**: Displayed on the top left screen.

- **Propulsion effect**: The jetpack displays a propulsion effect when it propels upwards.

- **Levels**: There are three levels in the game with speed increasing as the level goes up.

- **Glow effect**:  The zappers glow yellow and you character glow when their jetpack
is active. Used GLSL’s smoothstep for the same.

- **Running effect**:  The character runs as it moves on the ground.

## Built With

* C++
* OpenGL 3.0

