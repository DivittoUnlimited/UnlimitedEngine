#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
public:
    PauseState( States::ID id, StateStack& stack, Context context, bool allowUpdates = false );
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    GUI::Container  mGUIContainer;
    sf::Text        mPausedText;
    bool            mAllowUpdates;
};

#endif // PAUSESTATE_HPP
