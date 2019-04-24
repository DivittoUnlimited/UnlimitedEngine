#include "Bullet.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"

Bullet::Bullet(const TextureManager &textures )
    : Entity( )
{
    mSprite = sf::Sprite( textures.get( TextureMap.at( "Bullet" ) ) );
    centerOrigin( mSprite );
    mVelocity = sf::Vector2f( 0.0f, -200.0f );
}

bool Bullet::isMarkedForRemoval( ) const
{
    return false;
}

bool Bullet::isDestroyed( void ) const
{
    return false;
}

void Bullet::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Bullet::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    Entity::updateCurrent( dt, commands );
}
