# CHIP-8 Emulator

## Introduction

This is an interpreter for the CHIP-8 programming language developed by Joseph Weisbecker in the 1970's. I'm hoping to use this a springboard to later write a Gameboy emulator.

![Video Demo](/static/CHIP_8_ScreenRecording.mov)
![Video Demo](/static/CHIP_8_Breakout_ScreenRecording.mov)

## Table of Contents
- [Technologies Used](#technologies-used)
- [Building](#building)
- [Usage](#usage)
- [Project Status](#projectstatus)

## Technologies Used
* C++ 17
* SDL

## Building

1. Clone the repo

`git clone https://github.com/dustinburda/CHIP-8.git`

2. Install dependencies

SDL2: `brew install sdl2`
<br>
SDL2_TTF: `brew install sdl2_image`
<br>
SDL2_Image: `brew install sdl2_image`

3Run the follow commands to build the system
```
$ mkdir build
$ cd build
$ cmake ..
$ make 
```

4. Run the executable with your desired CHIP-8 ROM

`./CHIP-8 ./pong.ch8`





## Project Status
- Project is: _Done_.

