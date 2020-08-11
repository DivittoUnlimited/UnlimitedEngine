#ifndef ABILLITYSELECTMENUSTATE_HPP
#define ABILLITYSELECTMENUSTATE_HPP
#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class World;

class AbilitySelectMenuState : public State
{
public:
    AbilitySelectMenuState( States::ID id, StateStack& stack, Context context, World* world );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    GUI::Container      mGUIContainer;
    sf::Text            mText;
    sf::RectangleShape  mBackground;
    World* mWorld;
};

#endif // ABILLITYSELECTMENUSTATE_HPP
