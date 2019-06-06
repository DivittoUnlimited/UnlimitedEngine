
#include <math.h>

#include "Objects/AIStarShipController.hpp"
#include "Objects/StarShip.hpp"
#include "Arena.hpp"
// Computes the bearing in degrees from the point A(a1,a2) to
// the point B(b1,b2). Note that A and B are given in terms of
// screen coordinates.
static double bearing( double a1, double a2, double b1, double b2 ) {
    static const double TWOPI = 6.2831853071795865;
    static const double RAD2DEG = 57.2957795130823209;
    // if (a1 = b1 and a2 = b2) throw an error
    double theta = atan2(b1 - a1, a2 - b2);
    if (theta < 0.0)
       theta += TWOPI;
    return RAD2DEG * theta;
}

template <class T>
void IdleState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    owner->rotateLeft( );
    owner->mNextState = AIStarShipState::Pursuit;
}

template<class T>
void IdleState<T>::onEnter( T*, void* )
{
    std::cout << "IdleState onEnter( ) entered." << std::endl;
}

template<class T>
void IdleState<T>::onExit( T* )
{
    std::cout << "IdleState onExit( ) entered" << std::endl;
}

// ====================================================================================
/// MoveToState
///
template<class T>
void MoveToState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    if( ( mStarShip->getPosition().x >= mTargetPos.x - mStarShip->speed( ) ) && ( mStarShip->getPosition().x <= mTargetPos.x + mStarShip->speed( ) ) &&
         ( mStarShip->getPosition().y >= mTargetPos.y - mStarShip->speed( ) ) && ( mStarShip->getPosition().y <= mTargetPos.y + mStarShip->speed( ) ) )
    {
        owner->mNextState = AIStarShipState::Idle;
        return;
    }

    mTargetAngle = bearing( mStarShip->getPosition().x, mStarShip->getPosition().y, mTargetPos.x, mTargetPos.y );
    float rot = mStarShip->getRotation() ;

    if( 180 > rot )
    {
        if( 180 > mTargetAngle )
        {
            if( rot < mTargetAngle - mStarShip->speed( ) )
                owner->rotateRight( );
            else if( rot > mTargetAngle + mStarShip->speed( ) )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }else { // 180 < target
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateLeft( );
            else
                owner->rotateRight( );
        }
    }else // 180 < rot
    {
        if( 180 > mTargetAngle )
        {
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateLeft( );
            else
                owner->rotateRight( );
        }else { // 180 < target
            if( rot < mTargetAngle - mStarShip->speed( ) )
                owner->rotateRight( );
            else if( rot > mTargetAngle + mStarShip->speed( ) )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }
    }
}

template<class T>
void MoveToState<T>::onEnter( T* owner, void* data )
{
    for( unsigned int i = 0; i < 3; ++i ) // 3 becuase thats the number of starships per team
    {
        if( owner->mIdentifier == static_cast<unsigned int>(ARENA->REDTEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->REDTEAM->starShips[i];
            break;
        }
        else if( owner->mIdentifier == static_cast<unsigned int>( ARENA->BLUETEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->BLUETEAM->starShips[i];
            break;
        }
    }
    assert( mStarShip != nullptr );

    // Get the needed angle to change to to reach target position
    mTargetPos = *static_cast<sf::Vector2f*>( data );
    mTargetAngle = bearing( mStarShip->getPosition().x, mStarShip->getPosition().y, mTargetPos.x, mTargetPos.y );
}

template<class T>
void MoveToState<T>::onExit( T* owner )
{
    std::cout << "ID: " << owner->getCategory() << "MoveToState onExit( ) Complete!" << std::endl;
}

// ===========================================================================================================
/// PursuitState
///
template <class T>
void PursuitState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    mTargetPos = mTarget->getPosition() + (mTarget->getVelocity( ) * (float)mLookAheadTime.asMilliseconds( )); // CHECK ME IF SOMETHING LOOKS DUMB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    mTargetAngle = bearing( mStarShip->getPosition().x, mStarShip->getPosition().y, mTargetPos.x, mTargetPos.y );

    if( ( mStarShip->getPosition().x >= mTargetPos.x - mStarShip->speed( ) ) && ( mStarShip->getPosition().x <= mTargetPos.x + mStarShip->speed( ) ) &&
         ( mStarShip->getPosition().y >= mTargetPos.y - mStarShip->speed( ) ) && ( mStarShip->getPosition().y <= mTargetPos.y + mStarShip->speed( ) ) )
    {
        owner->mNextState = AIStarShipState::Idle;
        return;
    }

    float rot = mStarShip->getRotation( );
    if( 180 > rot )
    {
        if( 180 > mTargetAngle )
        {
            if( rot < mTargetAngle - mStarShip->speed( ) )
                owner->rotateRight( );
            else if( rot > mTargetAngle + mStarShip->speed( ) )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }else { // 180 < target
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateLeft( );
            else
                owner->rotateRight( );
        }
    }else // 180 < rot
    {
        if( 180 > mTargetAngle )
        {
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateLeft( );
            else
                owner->rotateRight( );
        }else { // 180 < target
            if( rot < mTargetAngle - mStarShip->speed( ) )
                owner->rotateRight( );
            else if( rot > mTargetAngle + mStarShip->speed( ) )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }
    }

}

template<class T>
void PursuitState<T>::onEnter( T* owner, void* data )
{
    std::cout << "PursuitState onEnter( ) entered." << std::endl;
    for( unsigned int i = 0; i < 3; ++i ) // 3 becuase thats the number of starships per team
    {
        if( owner->mIdentifier == static_cast<unsigned int>(ARENA->REDTEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->REDTEAM->starShips[i];
            break;
        }
        else if( owner->mIdentifier == static_cast<unsigned int>( ARENA->BLUETEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->BLUETEAM->starShips[i];
            break;
        }
    }
    assert( mStarShip != nullptr );

    // Get the needed angle to change to to reach target position
    mTarget = static_cast<Entity*>( data );
    assert( mTarget );
}

template<class T>
void PursuitState<T>::onExit( T* )
{
    std::cout << "PursuitState onExit( ) entered" << std::endl;
}

// ====================================================================================
/// Evade State
///
template <class T>
void EvadeState<T>::update( sf::Time, CommandQueue&, T* owner )
{

}

template<class T>
void EvadeState<T>::onEnter( T* owner, void* )
{
    std::cout << "EvadeState onEnter( ) entered." << std::endl;
}

template<class T>
void EvadeState<T>::onExit( T* )
{
    std::cout << "Evadetate onExit( ) entered" << std::endl;
}

// ====================================================================================
/// ShootTargetState
///
///
template <class T>
void ShootTargetState<T>::update( sf::Time, CommandQueue&, T* owner )
{

}

template<class T>
void ShootTargetState<T>::onEnter( T* owner, void* data )
{
    for( unsigned int i = 0; i < 3; ++i ) // 3 becuase thats the number of starships per team
    {
        if( owner->mIdentifier == static_cast<unsigned int>(ARENA->REDTEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->REDTEAM->starShips[i];
            break;
        }
        else if( owner->mIdentifier == static_cast<unsigned int>( ARENA->BLUETEAM->starShips[i]->getIdentifier( ) ) )
        {
            mStarShip = ARENA->BLUETEAM->starShips[i];
            break;
        }
    }
    assert( mStarShip != nullptr );

    // Get the needed angle to change to to reach target position
    mTargetPos = *static_cast<sf::Vector2f*>( data );
}

template<class T>
void ShootTargetState<T>::onExit( T* owner )
{
    std::cout << "ID: " << owner->getCategory() << "MoveToState onExit( ) Complete!" << std::endl;
}
