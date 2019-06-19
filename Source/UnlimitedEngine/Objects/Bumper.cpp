#include <math.h>
#include "Bumper.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/Utility.hpp"
#include "Core/Globals.hpp"
#include "StarShip.hpp"
#include "Arena.hpp"

Bumper::Bumper( Category::Type category )
    : Entity( )
    , mSprite( sf::ConvexShape( 7 ) )
    , mCategory( category )
    , mSpeed( 5 )
    , mMaxVelocity( 200 )
{
    centerOrigin( mSprite );

    mSprite.setPoint( 0, sf::Vector2f(   0, 10 ) );
    mSprite.setPoint( 1, sf::Vector2f( -55, 25 ) );
    mSprite.setPoint( 2, sf::Vector2f( -55,  5 ) );
    mSprite.setPoint( 3, sf::Vector2f( -40, -1 ) );
    mSprite.setPoint( 4, sf::Vector2f(  40, -1 ) );
    mSprite.setPoint( 5, sf::Vector2f(  55,  5 ) );
    mSprite.setPoint( 6, sf::Vector2f(  55, 25 ) );

    mSprite.setOutlineThickness( 2 );
    mSprite.setOutlineColor( sf::Color::White );
    if( getCategory( ) == Category::BumperBlue )
    {
        mSprite.setFillColor( sf::Color::Blue );
    }
    else if( getCategory( ) == Category::BumperRed )
    {
        mSprite.setFillColor( sf::Color::Red );
        this->rotate( 180 );
    }
}

unsigned int Bumper::getCategory( ) const
{
    return mCategory;
}

sf::FloatRect Bumper::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Bumper::isMarkedForRemoval( ) const
{
    return false;
}

void Bumper::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    float bumperX = this->getPosition( ).x;

    float center  = WINDOW_WIDTH / 2;

    if( mCategory == Category::BumperBlue )
    {
        float playerX = ARENA->REDTEAM->starShips[0]->getPosition( ).x;
        // both ship and bumper are on the same side of screen or nearer the middle of the screen than the edge
        if( ( ( bumperX > center && ( playerX > center ) ) || ( ( bumperX < center ) && ( playerX < center ) ) ) ||
                (std::abs( bumperX - center ) < 200 && ( std::abs( playerX - center ) < 200 ) ) )
        {
            if( playerX < bumperX )
                this->accelerate( -1 * mSpeed, 0 );
            else if( playerX > bumperX )
                this->accelerate( mSpeed, 0 );
            else // special case in the beggining of the match
                this->setVelocity( std::abs( this->getVelocity().x ) - this->getVelocity().x, 0 );
        }
        else
        {
            if( playerX < bumperX )
                this->accelerate( mSpeed, 0 );
            else if( playerX > bumperX )
                this->accelerate( -1 * mSpeed, 0 );
        }
    }
    else // Red Bumper
    {
        float playerX = ARENA->BLUETEAM->starShips[0]->getPosition( ).x;
        // both ship and bumper are on the same side of screen or nearer the middle of the screen than the edge
        if( ( ( ( ( bumperX > center ) && (playerX > center ) ) ) || ( ( bumperX < center ) && ( playerX < center ) ) ) ||
                ( ( std::abs( bumperX - center ) < 200 ) && ( std::abs( playerX - center ) < 200 ) ) )
        {
            if( playerX < bumperX )
                this->accelerate( -1 * mSpeed, 0 );
            else if( playerX > bumperX )
                this->accelerate( mSpeed, 0 );
            else // special case in the beggining of the match
                this->setVelocity( std::abs( this->getVelocity().x ) - this->getVelocity().x, 0 );
        }
        else
        {
            if( playerX < bumperX )
                this->accelerate( mSpeed, 0 );
            else if( playerX > bumperX )
                this->accelerate( -1 * mSpeed, 0 );
        }
    }
    // Wrap screen
    if( this->getPosition( ).x > WINDOW_WIDTH + 50 )
       this->setPosition( -20, this->getPosition( ).y );
    else if( this->getPosition( ).x < -50 )
       this->setPosition( WINDOW_WIDTH+20, this->getPosition( ).y );

    // Validate isnt going to fast and correct if needed.
    float totalVelocity = (float)std::sqrt( (this->getVelocity( ).x*this->getVelocity( ).x) + (this->getVelocity( ).y*this->getVelocity( ).y) );
    if( totalVelocity > this->maximumVelocity( ) + this->getHitpoints( ) )
        this->setVelocity( this->getVelocity( ) * this->maximumVelocity( ) / totalVelocity );

    Entity::updateCurrent( dt, commands );
}

void Bumper::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

bool Bumper::isDestroyed( ) const
{
    return false;
}
