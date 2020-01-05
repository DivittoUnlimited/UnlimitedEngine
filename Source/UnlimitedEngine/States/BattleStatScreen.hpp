#ifndef BATTLESTATSCREEN_HPP
#define BATTLESTATSCREEN_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class BattleStatScreen : public State
{
public:
    BattleStatScreen( States::ID id, StateStack& stack, Context context );
    ~BattleStatScreen( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // BATTLESTATSCREEN_HPP
