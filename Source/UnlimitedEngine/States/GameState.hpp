#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Game/World.hpp"
#include "Core/Player.hpp"

// Forward declaration
namespace sf
{
    class RenderTarget;
}

class GameState : public State
{
public:
    GameState( States::ID id, StateStack& stack, Context context );
    ~GameState( void );

    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

protected:
    World   mWorld;
    Player  mPlayer;
    unsigned int mLevel;
};

#endif // GAMESTATE_HPP
