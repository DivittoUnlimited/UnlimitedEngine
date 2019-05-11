#include "Flag.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/Utility.hpp"

Flag::Flag( bool isTeamAFlag, sf::Vector2f startingPos )
    : SceneNode( )
    , mSprite( sf::ConvexShape( 4 ) )
    , misTeamAFlag( isTeamAFlag )
    , mStarShip( nullptr )
    , mStartingPos( startingPos )
{
    centerOrigin( mSprite );
    mSprite.setPoint( 0, sf::Vector2f( -10, -10 ) );
    mSprite.setPoint( 1, sf::Vector2f(  10, -10 ) );
    mSprite.setPoint( 2, sf::Vector2f(  10,  10 ) );
    mSprite.setPoint( 3, sf::Vector2f( -10,  10 ) );

    if( isTeamAFlag )
        mSprite.setFillColor( sf::Color( 0, 0, 255, 200 ) );
    else
        mSprite.setFillColor( sf::Color( 255, 0, 0, 200 ) );

    this->setPosition( mStartingPos );
}

unsigned int Flag::getCategory( ) const
{
    if( this->misTeamAFlag )
        return Category::FlagA;
    return Category::FlagB;
}

sf::FloatRect Flag::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Flag::isMarkedForRemoval( ) const
{
    return false;
}

bool Flag::isDestroyed( ) const
{
    return false;
}

void Flag::updateCurrent( sf::Time, CommandQueue& )
{

}

void Flag::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}


