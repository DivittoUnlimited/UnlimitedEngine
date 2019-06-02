
#include <math.h>

#include "Objects/AIStarShipController.hpp"
#include "Objects/StarShip.hpp"
#include "Arena.hpp"


template <class T>
void IdleState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    owner->rotateLeft( );
    owner->mNextState = AIStarShipState::MoveTo;
}

template<class T>
void IdleState<T>::onEnter( T* owner, void* )
{
    std::cout << "IdleState onEnter( ) entered." << std::endl;
}

template<class T>
void IdleState<T>::onExit( T* )
{
    std::cout << "IdleState onExit( ) entered" << std::endl;
}

// ====================================================================================

template<class T>
void MoveToState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    std::cout << mTargetAngle << std::endl;
    float rot = mStarShip->getRotation() * ( 3.14f / 180.0f );
   owner->rotateRight();

    owner->thrust();
}
// Computes the bearing in degrees from the point A(a1,a2) to
// the point B(b1,b2). Note that A and B are given in terms of
// screen coordinates.
static double bearing(double a1, double a2, double b1, double b2) {
    static const double TWOPI = 6.2831853071795865;
    static const double RAD2DEG = 57.2957795130823209;
    // if (a1 = b1 and a2 = b2) throw an error
    double theta = atan2(b1 - a1, a2 - b2);
    if (theta < 0.0)
        theta += TWOPI;
    return RAD2DEG * theta;
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
    mTargetAngle = bearing( mStarShip->getPosition().x, mStarShip->getPosition().y, mTargetPos.x, mTargetPos.y );
    /*
    mTargetPos = *static_cast<sf::Vector2f*>( data );
    std::cout << "Attempting to MoveTo: ( X = " << mTargetPos.x << " Y = " << mTargetPos.y << " )" << std::endl;
    std::cout << "SLOPE: " << mTargetPos.y - mStarShip->getPosition( ).y << " / " << mTargetPos.x - mStarShip->getPosition( ).x << std::endl;
    mTargetAngle = std::atan( (mTargetPos.y - mStarShip->getPosition( ).y) / ( mTargetPos.x - mStarShip->getPosition( ).x ) );
    */
}

template<class T>
void MoveToState<T>::onExit( T* owner )
{
    std::cout << "ID: " << owner->getCategory() << "MoveToState onExit( ) Complete!" << std::endl;
}
