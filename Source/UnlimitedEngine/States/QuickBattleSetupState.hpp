#ifndef QUICKBATTLESETUPSTATE_HPP
#define QUICKBATTLESETUPSTATE_HPP


#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class QuickBattleSetupState : public State
{
public:
    QuickBattleSetupState( States::ID id, StateStack& stack, Context context );
    ~QuickBattleSetupState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // QUICKBATTLESETUPSTATE_HPP