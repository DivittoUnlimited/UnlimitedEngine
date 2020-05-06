#ifndef SHOPSTATE_HPP
#define SHOPSTATE_HPP



#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ShopState : public State
{
public:
    ShopState( States::ID id, StateStack& stack, Context context );
    ~ShopState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // SHOPSTATE_HPP
