#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Core/World.hpp"
#include "Core/Player.hpp"
#include "Objects/AIStarShipController.hpp"

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
    //Player*  mPlayer2;
    // AIStarShipController mBlue2;

    AIStarShipController mRed1;
    //AIStarShipController mRed2;

};

#endif // GAMESTATE_HPP
