#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "Core/State.hpp"
#include "Core/Player.hpp"
#include "Gui/Container.hpp"
#include "Gui/Button.hpp"
#include "Gui/Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>

class SettingsState : public State
{
public:
    SettingsState( States::ID id, StateStack& stack, Context context );
    ~SettingsState( void );

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    void updateLabels();
    void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);

    //## Attributes
    sf::Sprite										  mBackgroundSprite;
    GUI::Container									  mGUIContainer;
    std::array<GUI::Button::Ptr, Player::ActionCount> mBindingButtons;
    std::array<GUI::Label::Ptr, Player::ActionCount>  mBindingLabels;
};

#endif // SETTINGSSTATE_HPP
