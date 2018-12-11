#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP

#include "Core/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


class SplashScreen : public State
{
public:
    SplashScreen( States::ID id, StateStack& stack, Context context );
    ~SplashScreen( void );

    virtual void draw(  );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Sprite mLogo;
    sf::Time mElapsedTime;
};

#endif // SPLASHSCREEN_HPP
