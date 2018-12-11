#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <string>

//const static int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
//const static int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
const static int   WINDOW_HEIGHT = 768;
const static int   WINDOW_WIDTH  = 1024;
const static float FRAMES_PER_SECOND = 1.0f / 60.0f;

extern float GAME_SPEED;
extern unsigned int   WALL_DEPTH;
extern bool  FLIP_SCREEN;
extern bool  FLIP_GAMEPLAY;
extern bool  ALREADY_FLIPPED;
extern int   PLAYER_SCORE;
extern float BASE_SPEED;
extern int GAME_LEVEL;
extern int PLAYER_LIVES;
extern int TOTAL_PLAYER_SCORE;

#endif // GLOBALS_HPP
