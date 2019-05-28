#include "AIStarShipController.hpp"
#include "Objects/StarShip.hpp"
#include <math.h>



///
/// \brief The ActorMover struct
/// Functor to move the player based on velocity and the delta time for this frame.
struct StarShipMover
{
    StarShipMover( float vx, float vy )
    : velocity( vx, vy )
    {
    }
    void operator( )( StarShip& starShip, sf::Time ) const
    {
        starShip.accelerate( velocity * starShip.speed( ) );
    }
    sf::Vector2f velocity;
};

struct AircraftFireTrigger
{
    AircraftFireTrigger(int identifier)
    : ID(identifier)
    {
    }

    void operator( ) ( StarShip& starShip, sf::Time ) const
    {
        if( starShip.getIdentifier( ) == ID )
            starShip.fire( );
    }

    int ID;
};


AIStarShipController::AIStarShipController( unsigned int identifier )
    : mFsm( AI::FiniteStateMachine<AIStarShipController>( this, new IdleState<AIStarShipController>(  ) ) )
    , mIdentifier( identifier )
    , mMoveLeftFlag( false )
    , mMoveRightFlag( false )
    , mThrustFlag( false )
    , mFireFlag( false )
{
    // define commands
}

void AIStarShipController::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    // eval commands and act accordingly
    if( mMoveLeftFlag )
    {
        commands.push( mMoveLeftCommand );
        mMoveLeftFlag = false;
    }
    else if( mMoveRightFlag )
    {
        commands.push( mMoveRightCommand );
        mMoveRightFlag = false;
    }
    if( mThrustFlag )
    {
        commands.push( mThrustCommand );
        mThrustFlag = false;
    }
    if( mFireFlag )
    {
        commands.push( mFireCommand );
        mFireFlag = false;
    }
}

unsigned int AIStarShipController::getCategory( void ) const
{
    return mIdentifier; // CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
}

void AIStarShipController::initializeActions( )
{
    mActionBinding[PlayerAction::MoveLeft].action      = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == (unsigned int)a.getIdentifier( ) )
                a.rotate( -1 * a.speed( ) );
    } );
    mActionBinding[PlayerAction::MoveRight].action     = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == (unsigned int)a.getIdentifier( ) )
                a.rotate( a.speed( ) );
    } );
     mActionBinding[PlayerAction::MoveUp].action        = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == (unsigned int)a.getIdentifier( ) )
            {
                // break down angle to x and y
                double angle = a.getRotation() * (3.14 / 180);
                sf::Vector2f force;
                force.x = sin( angle );
                force.y = -cos( angle );
                // apply speed to components seperatly
                force *= a.speed( );
                // accellerate the ship with the results
                a.accelerate( force );
                // Validate ship isnt going to fast and correct if needed.
                float totalVelocity = (float)std::sqrt( (a.getVelocity( ).x*a.getVelocity( ).x) + (a.getVelocity( ).y*a.getVelocity( ).y) );
                if( totalVelocity > a.maximumVelocity( ) + a.getHitpoints( ) )
                    a.setVelocity( a.getVelocity( ) * a.maximumVelocity( ) / totalVelocity );
            }
    } );
    mActionBinding[PlayerAction::Fire].action         = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ) {
            if( this->mIdentifier == (unsigned int)a.getIdentifier() ) a.fire( );
    });
}
