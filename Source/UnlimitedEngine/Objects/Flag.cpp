#include "Flag.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/Utility.hpp"

Flag::Flag( Category::Type category, sf::Vector2f startingPos )
    : Entity( )
    , mSprite( sf::ConvexShape( 4 ) )
    , mCategory( category )
    , mStarShip( nullptr )
    , mStartingPos( startingPos )
    , mCapturable( true )
{
    centerOrigin( mSprite );
    mSprite.setPoint( 0, sf::Vector2f( -5, -5 ) );
    mSprite.setPoint( 1, sf::Vector2f(  5, -5 ) );
    mSprite.setPoint( 2, sf::Vector2f(  5,  5 ) );
    mSprite.setPoint( 3, sf::Vector2f( -5,  5 ) );

    mSprite.setOutlineThickness( 2 );
    mSprite.setOutlineColor( sf::Color::White );
    if( mCategory == Category::FlagBlue )
        mSprite.setFillColor( sf::Color( 0, 0, 255, 200 ) );
    else
        mSprite.setFillColor( sf::Color( 255, 0, 0, 200 ) );

    this->setPosition( mStartingPos );
}

unsigned int Flag::getCategory( ) const
{
    return mCategory;
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
    this->rotate( 4 );
}

void Flag::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}


