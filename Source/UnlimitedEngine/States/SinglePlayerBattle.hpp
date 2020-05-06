#ifndef SINGLEPLAYERBATTLE_HPP
#define SINGLEPLAYERBATTLE_HPP

#include "Game/World.hpp"
#include "Core/Player.hpp"
#include "Game/WifeBot.hpp"

// Forward declaration
namespace sf
{
    class RenderTarget;
}

class SinglePlayerBattle : public State
{
public:
    SinglePlayerBattle( States::ID id, StateStack& stack, Context context, unsigned int level );
    ~SinglePlayerBattle( void );

    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

protected:
    World           mWorld;
    Player          mPlayer;
    unsigned int    mLevel;
    WifeBot*         mWifeBot;
};

#endif // SINGLEPLAYERBATTLE_HPP
