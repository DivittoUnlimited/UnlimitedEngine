
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
    ///
    /// \brief SteeringBehaviors::seek
    /////     // SEEK WORKS BUT DOES NOT ACCOUNT FOR SCREENWRAP!!!!!!!!!!!!!
    SteeringBehaviors::seek( ARENA->REDTEAM->starShips.at(owner->getCategory( )), ARENA->BLUETEAM->starShips.at(Category::Blue1)->getPosition( ) );
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

// ===========================================================================================================
/// PursuitState
///
template <class T>
void PursuitState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    // save typing and method calls
    float x = mStarShip->getPosition( ).x;
    float y = mStarShip->getPosition( ).y;
    mTargetPos = owner->mTargetObject->getPosition( ) + (owner->mTargetObject->getVelocity( ) * static_cast<float>( mLookAheadTime.asSeconds( ) ) ); // CHECK ME IF SOMETHING LOOKS DUMB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    /*
    // if owner has met goal of colliding with target
    if( ( x >= mTargetPos.x - mStarShip->speed( ) ) && ( x <= mTargetPos.x + mStarShip->speed( ) ) &&
         ( y >= mTargetPos.y - mStarShip->speed( ) ) && ( y <= mTargetPos.y + mStarShip->speed( ) ) )
    {
        // owner->mNextState = AIStarShipState::Idle;
        return;
    }
*/

    // Solve so AI can use Screen wrap effect
    if( x > mTargetPos.x && ( ( WINDOW_WIDTH - x ) + mTargetPos.x < x - mTargetPos.x ) ) // if distance to target is shorter using wrap, owner to the right
    {
        mTargetAngle = bearing( -x, y, mTargetPos.x, mTargetPos.y );
    }
    else if( x < mTargetPos.x && ( ( WINDOW_WIDTH - mTargetPos.x ) + x < mTargetPos.x - x ) ) // if distance to target is shorter using wrap, owner to the left
    {
        mTargetAngle = bearing( x, y, -mTargetPos.x, mTargetPos.y );
    }
    else // ships closer not using wrap effect, solve normally
    {
        mTargetAngle = bearing( x, y, mTargetPos.x, mTargetPos.y );
    }

    float rot = mStarShip->getRotation( );
    if( 180 >= rot )
    {
        if( 180 >= mTargetAngle )
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
            else if( (360 - mTargetAngle) + rot > 180 )
                owner->rotateRight( );
            else {
                owner->thrust( );
            }
        }
    }
    else // 180 < rot
    {
        if( 180 >= mTargetAngle )
        {
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateRight( );
            else if( (360 - mTargetAngle) + rot > 180 )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }else if( 180 < mTargetAngle )
        {

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
    mStarShip = ARENA->REDTEAM->starShips[owner->getCategory( )];
    assert( mStarShip != nullptr );

    // Get the needed angle to change to to reach target position
    owner->mTargetObject = static_cast<Entity*>( data );
    assert( owner->mTargetObject );
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
    // save typing and method calls
    float x = mStarShip->getPosition( ).x;
    float y = mStarShip->getPosition( ).y;
    mTargetPos = owner->mTargetObject->getPosition( ) + (owner->mTargetObject->getVelocity( ) * static_cast<float>( mLookAheadTime.asSeconds( ) ) ); // CHECK ME IF SOMETHING LOOKS DUMB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // if owner has met goal of colliding with target
    if( ( x >= mTargetPos.x - mStarShip->speed( ) ) && ( x <= mTargetPos.x + mStarShip->speed( ) ) &&
         ( y >= mTargetPos.y - mStarShip->speed( ) ) && ( y <= mTargetPos.y + mStarShip->speed( ) ) )
    {
        owner->mNextState = AIStarShipState::Idle;
        return;
    }

    // Solve so AI can use Screen wrap effect
    if( x > mTargetPos.x && ( ( WINDOW_WIDTH - x ) + mTargetPos.x < x - mTargetPos.x ) ) // if distance to target is shorter using wrap, owner to the right
    {
        mStarShip->getSprite( )->setFillColor( sf::Color::Green );
        mTargetAngle = bearing( -x, y, mTargetPos.x, mTargetPos.y );
    }
    else if( x < mTargetPos.x && ( ( WINDOW_WIDTH - mTargetPos.x ) + x < mTargetPos.x - x ) ) // if distance to target is shorter using wrap, owner to the left
    {
        mStarShip->getSprite( )->setFillColor( sf::Color::Yellow );
        mTargetAngle = bearing( x, y, -mTargetPos.x, mTargetPos.y );
    }
    else // ships closer not using wrap effect, solve normally
    {
        mStarShip->getSprite( )->setFillColor( sf::Color::Red );
        mTargetAngle = bearing( x, y, mTargetPos.x, mTargetPos.y );
    }

    if( mTargetAngle < 180 )
        mTargetAngle += 180;
    else if( mTargetAngle >= 180 )
        mTargetAngle -= 180;

    float rot = mStarShip->getRotation( );
    if( 180 >= rot )
    {
        if( 180 >= mTargetAngle )
        {
            if( rot < mTargetAngle - mStarShip->speed( ) )
                owner->rotateRight( );
            else if( rot > mTargetAngle + mStarShip->speed( ) )
                owner->rotateLeft( );
            else {
                owner->thrust( );
            }
        }else { // 180 < target
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateLeft( );
            else if( (360 - mTargetAngle) + rot > 180 )
                owner->rotateRight( );
            else {
                owner->thrust( );
            }
        }
    }
    else // 180 < rot
    {
        if( 180 >= mTargetAngle )
        {
            if( (360 - mTargetAngle) + rot < 180 )
                owner->rotateRight( );
            else if( (360 - mTargetAngle) + rot > 180 )
                owner->rotateLeft( );
            else
                owner->thrust( );
        }else if( 180 < mTargetAngle )
        {

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
void EvadeState<T>::onEnter( T* owner, void* data )
{
    std::cout << "EvadeState onEnter( ) entered." << std::endl;
    mStarShip = ARENA->REDTEAM->starShips[owner->getCategory( )];
    assert( mStarShip != nullptr );

    // Get the needed angle to change to to reach target position
    owner->mTargetObject = static_cast<Entity*>( data );
    assert( owner->mTargetObject );
}

template<class T>
void EvadeState<T>::onExit( T* )
{
    std::cout << "EvadeState onExit( ) entered" << std::endl;
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
    mDistanceToTarget = std::sqrt( ( yComp / xComp ) );

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

    owner->mTargetObject = static_cast<Entity*>( data );
    assert( owner->mTargetObject );
    owner->mNextBlipState = AIStarShipState::Pursuit;
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
    SteeringBehaviors::seek( mStarShip, sf::Vector2f( 500, 500 ) );
   /*
    if( !mChangeTargets )
    {
        // Owner already has flag check if goal has been reached.
            // Clear states.

    }
    else if( mHasFlag && mChangeTargets )
    {
        // MoveTo goal

        mChangeTargets = false;
    }
    */
}

template<class T>
void CaptureFlagState<T>::onEnter( T* owner, void* data )
{
    std::cout << "CaptureFlagState onEnter( ) entered." << std::endl;
    mStarShip = ARENA->REDTEAM->starShips[(unsigned int)owner->getCategory( )];
    assert( mStarShip != nullptr );

    owner->mTargetObject = static_cast<Entity*>( data );
    assert( owner->mTargetObject );
}

template<class T>
void CaptureFlagState<T>::onExit( T* )
{
    std::cout << "CaptureFlagState onExit( ) entered" << std::endl;
}


































