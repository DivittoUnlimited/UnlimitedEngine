#ifndef WAITMENU_HPP
#define WAITMENU_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class World;

class WaitMenu : public State
{
public:
    WaitMenu( States::ID id, StateStack& stack, Context context, World* world );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
private:
    GUI::Container      mGUIContainer;
};

#endif // WAITMENU_HPP
