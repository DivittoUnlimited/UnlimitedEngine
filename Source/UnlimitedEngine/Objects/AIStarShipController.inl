
#include <math.h>

#include "Objects/AIStarShipController.hpp"
#include "Objects/StarShip.hpp"
#include "Arena.hpp"

template <class T>
void IdleState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    // still testing using the seek behavior for pursuit to just need to tweak it by a better lookahead value (2)
    owner->mNextState = AIStarShipState::CaptureFlag;
    // SteeringBehaviors::seek( ARENA->REDTEAM->starShips.at( owner->getCategory( ) ), ARENA->BLUETEAM->starShips.at( Category::Blue1 )->getPosition( ) + (ARENA->BLUETEAM->starShips.at( Category::Blue1 )->getVelocity( ) * 2.0f) );
}

template<class T>
void IdleState<T>::onEnter( T* owner, void* )
{
    std::cout << owner->getCategory( ) << " IdleState onEnter( ) entered." << std::endl;
}

template<class T>
void IdleState<T>::onExit( T* )
{
    std::cout << "IdleState onExit( ) entered" << std::endl;
}

// ====================================================================================
/// ShootTargetState
///
///
template <class T>
void ShootTargetState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    // solve distance to target
    float xComp = owner->mTargetObject->getPosition( ).x - mStarShip->getPosition( ).x;
    float yComp = owner->mTargetObject->getPosition( ).y - mStarShip->getPosition( ).y;
    xComp *= xComp;
    yComp *= yComp;
    mDistanceToTarget = std::sqrt( ( yComp + xComp ) );

    SteeringBehaviors::seek( mStarShip, owner->mTargetObject->getPosition( ) + (owner->mTargetObject->getVelocity( ) * 2.0f) );

    if( mDistanceToTarget <= mRange )
    {
        // shoot
        owner->fire( );
    }
}

template<class T>
void ShootTargetState<T>::onEnter( T* owner, void* data )
{
    mStarShip = ARENA->REDTEAM->starShips[owner->getCategory( )];
    assert( mStarShip != nullptr );

    owner->mTargetObject = ARENA->BLUETEAM->starShips[*static_cast<unsigned int*>( data )];

    assert( owner->mTargetObject );
}

template<class T>
void ShootTargetState<T>::onExit( T* owner )
{
    std::cout << "ID: " << owner->getCategory() << "ShootTargetState onExit( ) Complete!" << std::endl;
}

// ======================================================================================
/// CaptureFlagState
///
///
template <class T>
void CaptureFlagState<T>::update( sf::Time, CommandQueue&, T* owner )
{   
    if( mStarShip->mHasFlag && !mAlreadyHadFlag )
    {
        owner->mTargetObject = static_cast<Entity*>( ARENA->REDTEAM->goal );
        mAlreadyHadFlag = true;
    }
    else if( !mStarShip->mHasFlag && mAlreadyHadFlag )
    {
        mAlreadyHadFlag = false;
    }
    SteeringBehaviors::arrive( mStarShip, owner->mTargetObject->getPosition( ) );
}

template<class T>
void CaptureFlagState<T>::onEnter( T* owner, void* data )
{
    std::cout << "CaptureFlagState onEnter( ) entered." << std::endl;
    mStarShip = ARENA->REDTEAM->starShips[(unsigned int)owner->getCategory( )];
    assert( mStarShip != nullptr );

    owner->mTargetObject = ARENA->BLUETEAM->flags[owner->getNearestFlag( )];
    assert( owner->mTargetObject );
}

template<class T>
void CaptureFlagState<T>::onExit( T* )
{
    std::cout << "CaptureFlagState onExit( ) entered" << std::endl;
}


































