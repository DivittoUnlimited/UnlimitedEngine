#ifndef STORYMODESETUPSTATE_HPP
#define STORYMODESETUPSTATE_HPP


#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class StoryModeSetupState : public State
{
public:
    StoryModeSetupState( States::ID id, StateStack& stack, Context context );
    ~StoryModeSetupState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // STORYMODESETUPSTATE_HPP
