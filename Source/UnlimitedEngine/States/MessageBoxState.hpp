#ifndef MESSAGEBOXSTATE_HPP
#define MESSAGEBOXSTATE_HPP

#include "Core/State.hpp"
#include "Gui/MessageBoxNode.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MessageBoxState : public State
{
public:
    MessageBoxState( States::ID id, StateStack& stack, Context context );
    MessageBoxState( States::ID id, StateStack& stack, Context context, std::string convoName );
    ~MessageBoxState( void );

    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    MessageBoxNode* mMessageBox;
};

#endif // MESSAGEBOXSTATE_HPP

