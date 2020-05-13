#ifndef ACTIONMENUSTATE_HPP
#define ACTIONMENUSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class World;

class ActionMenuState : public State
{
public:
    ActionMenuState( States::ID id, StateStack& stack, Context context, World* world );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    GUI::Container      mGUIContainer;
    World* mWorld;
};
#endif // ACTIONMENUSTATE_HPP
