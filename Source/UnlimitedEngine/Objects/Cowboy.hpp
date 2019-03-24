#ifndef COWBOY_HPP
#define COWBOY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Objects/Actor.hpp"

class Cowboy : public Actor
{
public:
    Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts );
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( void ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
private:
    sf::CircleShape mLeftArm;
    sf::CircleShape mRightArm;

    sf::CircleShape mLeftLeg;
    sf::CircleShape mRightLeg;

    std::string     mCurrentAnimation;
    unsigned int    mCurrentAnimationFrame;
    sf::Time        mCurrentFrameTime;
};

#endif // COWBOY_HPP
