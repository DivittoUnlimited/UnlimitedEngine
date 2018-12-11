#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP

#include "Core/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State
{
public:
    TitleState( States::ID id, StateStack& stack, Context context );

    ~TitleState( );

    //## Interface
    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );
private:
    //## Attributes
    sf::Sprite mBackgroundSprite;
    sf::Text   mText;
    bool	   mShowText;
    sf::Time   mTextEffectTime;
    sf::Text   mTitleText;
    sf::Time   mTimeOut;
};

#endif // TITLESTATE_HPP
