#ifndef CONQUESTBATTLESETUPSTATE_HPP
#define CONQUESTBATTLESETUPSTATE_HPP



#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ConquestBattleSetupState : public State
{
public:
    ConquestBattleSetupState( States::ID id, StateStack& stack, Context context );
    ~ConquestBattleSetupState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // CONQUESTBATTLESETUPSTATE_HPP
