#include "Bullet.hpp"
#include "Core/Globals.hpp"

#include <math.h>

Bullet::Bullet( sf::Vector2f pos, sf::Vector2f vel, float angle )
    : Entity( )
    , mStartingLifeSpan( sf::milliseconds( 2000 ) )
    , mLifeSpan( mStartingLifeSpan )
    , mSprite( sf::CircleShape( 4 ) )
    , mMaxVelocity( 1800 )
{
    mSprite.setFillColor( sf::Color::Yellow );
    this->setPosition( pos + sf::Vector2f( 0, -20 ) );
    angle *= (3.14 / 180);
    sf::Vector2f force( std::sin( angle ), -1 * std::cos( angle ) );
    force *= 300.0f;
    force += vel;
    this->setVelocity( force );
}

unsigned int Bullet::getCategory( ) const
{
    return Category::Bullet;
}

sf::FloatRect Bullet::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Bullet::isMarkedForRemoval( ) const
{
    return ( mLifeSpan <= sf::Time::Zero );
}

bool Bullet::isDestroyed( ) const
{
    return ( mLifeSpan <= sf::Time::Zero );
}

void Bullet::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    mLifeSpan -= dt;
    if( this->getPosition().y < 0 || this->getPosition().y > WINDOW_HEIGHT ) this->mLifeSpan = sf::Time::Zero;

    if( isMarkedForRemoval( ) ) return;

    if( this->getPosition( ).x > WINDOW_WIDTH + 30 ) this->setPosition( -30, this->getPosition( ).y );
    else if( this->getPosition( ).x < -30 ) this->setPosition( WINDOW_WIDTH + 30, this->getPosition( ).y );

    Entity::updateCurrent( dt, commands ) ;
}

void Bullet::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
