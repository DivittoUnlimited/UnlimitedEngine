#include "StarShip.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Core/Utility.hpp"

StarShip::StarShip( bool isPlayer )
    : Entity( )
    , mSprite( sf::ConvexShape( 4 ) )
    , mIsPlayer( isPlayer )
    , mSpeed( 5 )
    , mMaxVelocity( 200 )
{
    centerOrigin( mSprite );
    mSprite.setPoint( 0, sf::Vector2f(  0, -15 ) );
    mSprite.setPoint( 1, sf::Vector2f(  8,  12 ) );
    mSprite.setPoint( 2, sf::Vector2f(  0,   5 ) );
    mSprite.setPoint( 3, sf::Vector2f( -8,  12 ) );
    mSprite.setFillColor( sf::Color::Blue );
}

unsigned int  StarShip::getCategory( ) const
{
    if( mIsPlayer ) return Category::Player;
    return Category::StarShipB;
}

sf::FloatRect StarShip::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool StarShip::StarShip::isMarkedForRemoval( ) const
{
    return false;
}

void StarShip::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    Entity::updateCurrent( dt, commands );
}

void StarShip::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
