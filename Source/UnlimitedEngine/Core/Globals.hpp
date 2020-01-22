#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Core/GameModeIdentifiers.hpp"
#include "Core/StateIdentifiers.hpp"

//const static int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
//const static int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
const static int   WINDOW_HEIGHT = 768;
const static int   WINDOW_WIDTH  = 1024;
const static float FRAMES_PER_SECOND = 1.0f / 60.0f;

extern GameModes::ID GAME_MODE; // init in Application Constructor

extern sf::RenderWindow* mWindow; // The Main window of the game. Defined in Application Contructor.

extern States::ID CURRENT_LEVEL; // init in application constructor


#endif // GLOBALS_HPP
