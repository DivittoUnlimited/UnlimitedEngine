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
    ~MenuState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    sf::Text       mDevNotes;
    GUI::Container mGUIContainer;
    sf::Time       mTimeOut;
};

#endif // BOOK_MENUSTATE_HPP
