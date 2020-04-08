#ifndef ENDTURNMENUSTATE_HPP
#define ENDTURNMENUSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"
#include "Game/World.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class EndTurnMenuState : public State
{
public:
    EndTurnMenuState(States::ID id, StateStack& stack, Context context, World* world );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    World* mWorld;

    ///
    /// \brief mShowChangeTurnGUI
    /// flag to draw gui to change turns or not
    bool                                mShowChangeTurnGUI;
    ///
    /// \brief mChangeTurnGUI
    /// Yes or no buttons that are prompted when the user presses the enter/shift keys
    /// used to move to the next teams turn.
    GUI::Container mChangeTurnGUI;
    ///
    /// \brief mChangeTurnText
    /// The text above the buttons that informs the player they are aboutto end their turn.
    sf::Text   mChangeTurnText;

    sf::RectangleShape  mChangeTurnBackground;

};


#endif // ENDTURNMENUSTATE_HPP
