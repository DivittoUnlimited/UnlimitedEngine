#ifndef SINGLEPLAYERMENUSTATE_HPP
#define SINGLEPLAYERMENUSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SinglePlayerMenuState : public State
{
public:
    SinglePlayerMenuState( States::ID id, StateStack& stack, Context context );
    ~SinglePlayerMenuState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // SINGLEPLAYERMENUSTATE_HPP
