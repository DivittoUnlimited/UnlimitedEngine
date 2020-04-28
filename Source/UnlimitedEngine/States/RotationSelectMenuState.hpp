#ifndef ROTATIONSELECTMENUSTATE_HPP
#define ROTATIONSELECTMENUSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class World;

class RotationSelectMenuState : public State
{
public:
    RotationSelectMenuState( States::ID id, StateStack& stack, Context context, World* world );
    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    GUI::Container      mGUIContainer;
};

#endif // ROTATIONSELECTMENUSTATE_HPP
