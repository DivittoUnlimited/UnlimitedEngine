#ifndef BULLET_HPP
#define BULLET_HPP

#include "Core/DataTables.hpp"
#include "Core/Entity.hpp"

class Bullet : public Entity
{
public:
    Bullet( const TextureManager& textures );

    virtual bool isMarkedForRemoval( ) const;
    virtual bool isDestroyed( void ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
private:
    sf::Sprite mSprite;
};

#endif // BULLET_HPP
