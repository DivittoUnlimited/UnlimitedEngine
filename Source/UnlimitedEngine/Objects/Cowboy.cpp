#include "Cowboy.hpp"
#include "Objects/Bullet.hpp"
#include "Core/CommandQueue.hpp"
#include <memory>


namespace
{
    const auto Table = initializeScriptedAnimationData; // Gives access to the data in ScriptedAnimations.lua
}

Cowboy::Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts )
    : Actor( data, texID, texRect, textures, soundPlayer, fonts )
    , mReloadTimer( sf::Time::Zero )
    , mFireCommand( )
    , mPullTrigger( false )
{
    mFireCommand.category = Category::TileLayer;
    mFireCommand.action   = [this, &textures] ( SceneNode& node, sf::Time )
    {
        std::unique_ptr<Bullet> bullet( new Bullet( textures ) );
        bullet->setPosition( this->getWorldPosition( ).x, this->getWorldPosition().y - 100 );
        node.attachChild( std::move( bullet ) );
    };
}

bool Cowboy::isMarkedForRemoval( ) const
{
    return false;
}

bool Cowboy::isDestroyed( void ) const
{
    return false;
}

void Cowboy::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite,   states );
}

void Cowboy::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( mCanMove )
       updateMovementPattern( dt );


    if( mPullTrigger )
    {
        mPullTrigger = false;
        commands.push( mFireCommand );
    }

    if( mType == "Player" )
    {
        if( !mCanShoot )
        {
            mReloadTimer -= dt;
            if( mReloadTimer <= sf::Time::Zero )
            {
                mCanShoot = true;
            }
        }
    }


    Actor::updateCurrent( dt, commands );
}

void Cowboy::fire( void )
{
    if( mCanShoot )
    {
        mPullTrigger = true;
        mCanShoot = false;
        mReloadTimer += sf::milliseconds( 3000 );
    }
}

