#include "StarShip.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Core/Globals.hpp"
#include "Core/Utility.hpp"
#include "Core/CommandQueue.hpp"

StarShip::StarShip( Category::Type category )
    : Entity( )
    , mIdentifier( category )
    , mSprite( sf::ConvexShape( 4 ) )
    , mSpeed( 5 )
    , mMaxVelocity( 200 )
    , mFire( false )
    , mCategory( category )
    , mShotTimer( sf::Time::Zero )
    , mReloadTime( sf::milliseconds( 100 ) )
    , mHasFlag( false )
{
    centerOrigin( mSprite );
    mSprite.setPoint( 0, sf::Vector2f(  0, -12 ) );
    mSprite.setPoint( 1, sf::Vector2f(  6,  14 ) );
    mSprite.setPoint( 2, sf::Vector2f(  0,   3 ) );
    mSprite.setPoint( 3, sf::Vector2f( -6,  14 ) );
    mSprite.setOutlineThickness( 1 );
    mSprite.setOutlineColor( sf::Color::White );

    if( mCategory == Category::StarShipBlue || mCategory == Category::Player )
        mSprite.setFillColor( sf::Color::Blue );
    else if( mCategory == Category::StarShipRed || mCategory == Category::Player2 )
        mSprite.setFillColor( sf::Color::Red );
    else
        mSprite.setFillColor( sf::Color( 255, 0, 255, 0 ) );

    mFireCommand.category = Category::ObjectLayer;
    mFireCommand.action   = [this] ( SceneNode& node, sf::Time )
    {
         std::unique_ptr<Bullet> bullet( new Bullet( this->getPosition( ), this->getVelocity( ), this->getRotation( ) ) );

         bullet->setPosition( getWorldPosition( ) );
         node.attachChild( std::move( bullet ) );
    };
}

unsigned int  StarShip::getCategory( ) const
{
    return mCategory;
}

void StarShip::setCategory( Category::Type cat )
{
    this->mCategory = cat;
}

sf::FloatRect StarShip::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool StarShip::isMarkedForRemoval( ) const
{
    return false;
}

void StarShip::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( mShotTimer > sf::Time::Zero ) mShotTimer -= dt;
    else if( mFire ) {
        commands.push( mFireCommand );
        mFire = false;
    }

    if( this->getPosition( ).x > WINDOW_WIDTH + 30 )
        this->setPosition( -30, this->getPosition( ).y );
    else if( this->getPosition( ).x < -30 )
        this->setPosition( WINDOW_WIDTH + 30, this->getPosition( ).y );

    if( this->getPosition().y < 0 ) this->setPosition( this->getPosition().x, 0 );
    else if( this->getPosition().y > WINDOW_HEIGHT ) this->setPosition( this->getPosition().x, WINDOW_HEIGHT );

    Entity::updateCurrent( dt, commands );
}

void StarShip::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void StarShip::fire( void )
{
    if( mShotTimer <= sf::Time::Zero )
    {
        mShotTimer += mReloadTime;
        mFire = true;
    }
}

