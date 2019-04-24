#ifndef COWBOY_HPP
#define COWBOY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Time.hpp>

#include "Core/Command.hpp"
#include "Objects/Actor.hpp"

class Cowboy : public Actor
{
public:
    Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts );
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( void ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );

    void fire( void );

private:
    sf::Time mReloadTimer;
    bool mCanShoot;
    bool mPullTrigger;
    Command mFireCommand;
};

#endif // COWBOY_HPP
