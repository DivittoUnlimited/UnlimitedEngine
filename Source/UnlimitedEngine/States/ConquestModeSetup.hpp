#ifndef CONQUESTMODESETUP_HPP
#define CONQUESTMODESETUP_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class ConquestModeSetup : public State
{
public:
    ConquestModeSetup( States::ID id, StateStack& stack, Context context );
    ~ConquestModeSetup( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};


#endif // CONQUESTMODESETUP_HPP
