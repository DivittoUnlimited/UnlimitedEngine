#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Core/GameModeIdentifiers.hpp"

//const static int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
//const static int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
const static int   WINDOW_HEIGHT = 768;
const static int   WINDOW_WIDTH  = 1024;
const static float FRAMES_PER_SECOND = 1.0f / 60.0f;
static GameModes::ID GAME_MODE = GameModes::NONE;


#endif // GLOBALS_HPP
