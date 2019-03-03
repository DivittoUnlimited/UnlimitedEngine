#include "Cowboy.hpp"


Cowboy::Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts )
    : Actor( data, texID, texRect, textures, soundPlayer, fonts )
    , mLeftArm(  sf::CircleShape( 3 ) )
    , mRightArm( sf::CircleShape( 3 ) )
    , mLeftLeg(  sf::CircleShape( 4 ) )
    , mRightLeg( sf::CircleShape( 4 ) )
    , mCurrentAnimation( Animations::WalkRight )
    , mCurrentAnimationFrame( 0 )
    , mMaxFrameTime( sf::milliseconds( 300 ) )
    , mCurrentFrameTime( sf::Time::Zero )
    , mAnimations( )
{
    mLeftArm.setFillColor(  sf::Color::Red );
    mLeftLeg.setFillColor(  sf::Color::Red );
    mRightArm.setFillColor( sf::Color::Blue );
    mRightLeg.setFillColor( sf::Color::Blue );

    ///
    /// \brief standingAnimation
    /// WRITE ANIMATIONS LIKE THIS IN LUA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ///
    ///
    ///
    ///
    /// SIMPLFY ALL PAIRS AND CRAP TO SIMPLE NUMBERS LOAD TABLE STYLE
    /// DO NOT REMOVE CURRENT ANIMATIONS
    /// CALL THESE COWBOY ANIMATIONS OR SOMETHING!!!!!!!!!!!!!!!!!!!!!!!!!

    std::vector<AnimationFrame> standingAnimation;
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( 0, 0 ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 6 ), std::pair<float, float>( -2, 6 ), std::pair<float, float>( -2 ,20 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( 0, 0 ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 6 ), std::pair<float, float>( -2, 6 ), std::pair<float, float>( -2 ,20 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( 0, 0 ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( 0, 0 ) ) );
    mAnimations.insert( std::pair<Animations, std::vector<AnimationFrame>>( Animations::Standing, standingAnimation ) );

    std::vector<AnimationFrame> walkRightAnimation;
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>(  5, 7 ), std::pair<float, float>( -9, 7 ), std::pair<float, float>( -11, 16 ), std::pair<float, float>(   5, 20 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>(  3, 8 ), std::pair<float, float>( -7, 8 ), std::pair<float, float>(  -7, 16 ), std::pair<float, float>(   3, 20 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 9 ), std::pair<float, float>( -2, 9 ), std::pair<float, float>(  -5, 16 ), std::pair<float, float>(  -5, 20 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>( -7, 8 ), std::pair<float, float>(  3, 8 ), std::pair<float, float>(   3, 16 ), std::pair<float, float>(  -7, 20 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>( -9, 7 ), std::pair<float, float>(  5, 7 ), std::pair<float, float>(   5, 20 ), std::pair<float, float>( -11, 16 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>( -7, 8 ), std::pair<float, float>(  3, 8 ), std::pair<float, float>(   3, 20 ), std::pair<float, float>(  -7, 16 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 9 ), std::pair<float, float>( -2, 9 ), std::pair<float, float>(  -5, 20 ), std::pair<float, float>(  -5, 16 ), std::pair<float, float>( -2, 20 ) ) );
    walkRightAnimation.push_back( AnimationFrame( std::pair<float, float>(  3, 8 ), std::pair<float, float>( -7, 8 ), std::pair<float, float>(  -7, 20 ), std::pair<float, float>(   3, 16 ), std::pair<float, float>( -2, 20 ) ) );
    mAnimations.insert( std::pair<Animations, std::vector<AnimationFrame>>( Animations::WalkRight, walkRightAnimation ) );



    /*
    std::vector<AnimationFrame> standingAnimation;
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 8 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( -2, 20 ), std::pair<float, float>( -2, 20 ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ) ) );
    standingAnimation.push_back( AnimationFrame( std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ), std::pair<float, float>(  ) ) );
    mAnimations.insert( std::pair<Animations, std::vector<AnimationFrame>>( Animations::Standing, standingAnimation ) );
    */

    mLeftArm.setPosition( mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].leftArm.first, mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].leftArm.first );
    mRightArm.setPosition( mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].rightArm.first, mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].rightArm.first );
    mLeftLeg.setPosition( mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].leftLeg.first, mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].leftLeg.first );
    mRightLeg.setPosition( mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].rightLeg.first, mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame].rightLeg.first );



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
    // run next animation frame
    mCurrentFrameTime += dt;
    if( mCurrentFrameTime > mMaxFrameTime )
    {
        mCurrentFrameTime = sf::Time::Zero;
        if( mCurrentAnimationFrame < mAnimations.at( mCurrentAnimation ).size( ) - 1 )
            mCurrentAnimationFrame++;
        else
            mCurrentAnimationFrame = 0;

        auto frame = mAnimations.at( mCurrentAnimation )[mCurrentAnimationFrame];

        mLeftArm.setPosition(  frame.leftArm.first,  frame.leftArm.second  );
        mRightArm.setPosition( frame.rightArm.first, frame.rightArm.second );
        mRightLeg.setPosition( frame.rightLeg.first, frame.rightLeg.second );
        mLeftLeg.setPosition(  frame.leftLeg.first,  frame.leftLeg.second  );
    }
    Entity::updateCurrent( dt, commands );
}

