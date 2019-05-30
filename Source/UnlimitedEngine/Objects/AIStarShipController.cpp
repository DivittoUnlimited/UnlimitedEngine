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
    , mMoveLeftCommand( )
    , mMoveRightCommand( )
    , mThrustCommand( )
    , mFireCommand( )
{
    // define commands
    mMoveLeftCommand.category = mIdentifier;
    mMoveLeftCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == static_cast<unsigned int>(a.getIdentifier( ) ) )
                a.rotate( -1 * a.speed( ) );
    } );
    mMoveRightCommand.category = mIdentifier;
    mMoveRightCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == static_cast<unsigned int>(a.getIdentifier( ) ) )
                a.rotate( a.speed( ) );
    } );
    mThrustCommand.category = mIdentifier;
    mThrustCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == static_cast<unsigned int>(a.getIdentifier( ) ) )
            {
                // break down angle to x and y
                float angle = a.getRotation( ) * ( 3.14f / 180.0f );
                sf::Vector2f force;
                force.x = static_cast<float>( sin( angle ) );
                force.y = static_cast<float>( -cos( angle ) );
                // apply speed to components seperatly
                force *= a.speed( );
                // accellerate the ship with the results
                a.accelerate( force );
                // Validate ship isnt going to fast and correct if needed.
                float totalVelocity = static_cast<float>( std::sqrt( (a.getVelocity( ).x*a.getVelocity( ).x) + (a.getVelocity( ).y*a.getVelocity( ).y) ) );
                if( totalVelocity > a.maximumVelocity( ) + a.getHitpoints( ) )
                    a.setVelocity( a.getVelocity( ) * a.maximumVelocity( ) / totalVelocity );
            }
    } );
    mFireCommand.category = mIdentifier;
    mFireCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ) {
            if( this->mIdentifier == static_cast<unsigned int>( a.getIdentifier( ) ) ) a.fire( );
    });
}

void AIStarShipController::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    mFsm.update( dt, commands );
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
