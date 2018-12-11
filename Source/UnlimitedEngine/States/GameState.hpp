#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Core/State.hpp"
#include "Core/World.hpp"
#include "Core/Player.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
    GameState(States::ID id, StateStack& stack, Context context );

    ~GameState( );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

protected:
    World				mWorld;
    Player&				mPlayer;
};

#endif // GAMESTATE_HPP
