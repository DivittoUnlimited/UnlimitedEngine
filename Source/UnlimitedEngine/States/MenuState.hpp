#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
public:
    MenuState( States::ID id, StateStack& stack, Context context );
    ~MenuState();
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
    sf::Time       mTimeOut;
};

#endif // BOOK_MENUSTATE_HPP
