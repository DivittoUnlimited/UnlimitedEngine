#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SettingsState : public State
{
public:
    SettingsState( States::ID id, StateStack& stack, Context context );
    ~SettingsState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text       mTitleText;
    GUI::Container mGUIContainer;
};

#endif // SETTINGSSTATE_HPP
