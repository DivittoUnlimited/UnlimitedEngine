#include "Entity.hpp"

#include <cassert>


Entity::Entity( void )
: mVelocity( )
, mHitpoints( 100 )
{
}

void Entity::setVelocity( sf::Vector2f velocity )
{
    mVelocity = velocity;
}

void Entity::setVelocity( float vx, float vy )
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity( ) const
{
    return mVelocity;
}

void Entity::accelerate( sf::Vector2f velocity )
{
    mVelocity += velocity;
}

void Entity::accelerate( float vx, float vy )
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

bool Entity::isDestroyed( ) const
{
    return mHitpoints <= 0;
}

void Entity::updateCurrent( sf::Time dt, CommandQueue& )
{
    move( mVelocity * dt.asSeconds( )  );
}
