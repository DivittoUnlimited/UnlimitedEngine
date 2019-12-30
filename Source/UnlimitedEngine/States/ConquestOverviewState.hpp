#ifndef CONQUESTOVERVIEWSTATE_HPP
#define CONQUESTOVERVIEWSTATE_HPP



#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ConquestOverviewState : public State
{
public:
    ConquestOverviewState( States::ID id, StateStack& stack, Context context );
    ~ConquestOverviewState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};
#endif // CONQUESTOVERVIEWSTATE_HPP
