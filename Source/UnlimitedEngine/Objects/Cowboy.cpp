#include "Cowboy.hpp"

namespace
{
    const auto Table = initializeScriptedAnimationData; // Gives access to the data in ScriptedAnimations.lua
}

Cowboy::Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts )
    : Actor( data, texID, texRect, textures, soundPlayer, fonts )
    , mLeftArm(  sf::CircleShape( 0 ) )
    , mRightArm( sf::CircleShape( 0 ) )
    , mLeftLeg(  sf::CircleShape( 0 ) )
    , mRightLeg( sf::CircleShape( 0 ) )
    , mCurrentAnimationFrame( 0 )
    , mCurrentFrameTime( sf::Time::Zero )
{
    if( mType == "Player" )
        mCurrentAnimation = "walkRight";
    else if( mType == "Cowboy" )
        mCurrentAnimation = "standing";

    auto frame = Table[ScriptedAnimationMap.at(mCurrentAnimation)].frames[mCurrentAnimationFrame];

    mLeftArm.setPosition(  frame.leftArm.pos  );
    mRightArm.setPosition( frame.rightArm.pos );
    mRightLeg.setPosition( frame.rightLeg.pos );
    mLeftLeg.setPosition(  frame.leftLeg.pos  );

    mLeftArm.setRadius(  frame.leftArm.radius );
    mRightArm.setRadius( frame.rightArm.radius );
    mLeftLeg.setRadius(  frame.leftLeg.radius );
    mRightLeg.setRadius( frame.rightLeg.radius );

    mLeftArm.setFillColor(  frame.leftArm.color );
    mRightArm.setFillColor( frame.rightArm.color );
    mLeftLeg.setFillColor(  frame.leftLeg.color );
    mRightLeg.setFillColor( frame.rightLeg.color );
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
    target.draw( mLeftArm,  states );
    target.draw( mLeftLeg,  states );
    target.draw( mSprite,   states );
    target.draw( mRightArm, states );
    target.draw( mRightLeg, states );
}

void Cowboy::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( mCanMove )
       updateMovementPattern( dt );
    mCurrentFrameTime += dt;

    auto frame = Table[ScriptedAnimationMap.at( mCurrentAnimation )].frames[mCurrentAnimationFrame];
    if( mCurrentFrameTime.asMilliseconds( ) > frame.duration )
    {
        mCurrentFrameTime = sf::Time::Zero;
        // run next animation frame
        if( mCurrentAnimationFrame < Table[ScriptedAnimationMap.at( mCurrentAnimation )].frames.size( ) - 1 )
            mCurrentAnimationFrame++;
        else
            mCurrentAnimationFrame = 0;

        mLeftArm.setPosition(  frame.leftArm.pos  );
        mRightArm.setPosition( frame.rightArm.pos );
        mRightLeg.setPosition( frame.rightLeg.pos );
        mLeftLeg.setPosition(  frame.leftLeg.pos  );

        mLeftArm.setRadius(  frame.leftArm.radius );
        mRightArm.setRadius( frame.rightArm.radius );
        mLeftLeg.setRadius(  frame.leftLeg.radius );
        mRightLeg.setRadius( frame.rightLeg.radius );

        mLeftArm.setFillColor(  frame.leftArm.color );
        mRightArm.setFillColor( frame.rightArm.color );
        mLeftLeg.setFillColor(  frame.leftLeg.color );
        mRightLeg.setFillColor( frame.rightLeg.color );
    }
    Actor::updateCurrent( dt, commands );
}

