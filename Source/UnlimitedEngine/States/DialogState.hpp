#ifndef DIALOGSTATE_HPP
#define DIALOGSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class DialogState : public State
{
public:
    DialogState( States::ID id, StateStack& stack, Context context );
    ~DialogState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text             mTitleText;
    sf::Text             mMsg;
    sf::RectangleShape   mBackgroundBox;
    GUI::Container       mGUIContainer;
};

#endif // DIALOGSTATE_HPP
