#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <string>

/*
class Goal;
class StarShip;
class Bumper;
*/

class Arena;

//const static int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
//const static int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
const static int   WINDOW_HEIGHT = 768;
const static int   WINDOW_WIDTH  = 1024;
const static float FRAMES_PER_SECOND = 1.0f / 60.0f;

/*
extern StarShip* Blue;
extern StarShip* Red;

extern Goal* GoalBlue;
extern Goal* GoalRed;
*/
extern Arena* ARENA;
#endif // GLOBALS_HPP
