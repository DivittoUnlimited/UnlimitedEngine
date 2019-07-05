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
    : mFsm( AI::FiniteStateMachine<AIStarShipController>( this, new IdleState<AIStarShipController>( ) ) )
    , mIdentifier( identifier )
    , mMoveLeftFlag( false )
    , mMoveRightFlag( false )
    , mThrustFlag( false )
    , mFireFlag( false )
    , mNextState( AIStarShipState::None )
    , mNextBlipState( AIStarShipState::None )
    , mMoveLeftCommand( )
    , mMoveRightCommand( )
    , mThrustCommand( )
    , mFireCommand( )
    , mStarShip( ARENA->REDTEAM->starShips[mIdentifier] )
{
    // define commands
    mMoveLeftCommand.category = mIdentifier;
    mMoveLeftCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == static_cast<unsigned int>(a.getIdentifier( ) ) )
                a.rotate( -1 * a.speed( ) - 2 );
    } );
    mMoveRightCommand.category = mIdentifier;
    mMoveRightCommand.action = derivedAction<StarShip>( [this] ( StarShip& a, sf::Time ){
            if( this->mIdentifier == static_cast<unsigned int>(a.getIdentifier( ) ) )
                a.rotate( a.speed( ) + 2 );
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
                force *= a.speed( ) + 1;
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
    if( mNextState != AIStarShipState::None )
    {
        switch( mNextState )
        {
            case AIStarShipState::Idle:         mFsm.changeState( new IdleState<AIStarShipController>( ) );                                                                     break;
            case AIStarShipState::ShootTarget:  mFsm.changeState( new ShootTargetState<AIStarShipController>(  ), static_cast<void*>( new unsigned int( Category::Blue1 ) ) );  break;
            case AIStarShipState::CaptureFlag:  mFsm.changeState( new CaptureFlagState<AIStarShipController>(  ), static_cast<void*>( ARENA->BLUETEAM->flags[0] ) );            break;
            default: std::cout << "Invalid state reached in the AIStarshipController!" << std::endl;                                                                            break;
        }
        mNextState = AIStarShipState::None;
    }

    if( mNextBlipState != AIStarShipState::None )
    {
        switch( mNextBlipState ) {
            case AIStarShipState::Idle:         mFsm.enterBlipState( new IdleState<AIStarShipController>( ) );                                                              break;
            case AIStarShipState::ShootTarget:  mFsm.enterBlipState( new ShootTargetState<AIStarShipController>(  ), static_cast<void*>( ARENA->BLUETEAM->starShips[0] ) ); break;
            case AIStarShipState::CaptureFlag:  mFsm.enterBlipState( new CaptureFlagState<AIStarShipController>(  ), static_cast<void*>( ARENA->BLUETEAM->starShips[0] ) ); break;
            default: std::cout << "Invalid state reached in the AIStarshipController!" << std::endl;                                                                        break;
        }
        std::cout << "HEY!!!! Targeting Blue[0] is broken!!!! line 140" << std::endl;
        mNextBlipState = AIStarShipState::None;
    }
    mFsm.update( dt, commands );

    // eval commands and act accordingly
    /*
     *
     *
     *  NOT IN USE DONT GET CONFUSED
    if( mMoveLeftFlag )
    {
        commands.push( mMoveLeftCommand );
        mMoveLeftFlag = false;
        mMoveRightFlag = false; // just to clear everything
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
    */
    if( mFireFlag )
    {
        commands.push( mFireCommand );
        mFireFlag = false;
    }
}

unsigned int AIStarShipController::getCategory( void ) const
{
    return mIdentifier;
}

unsigned int AIStarShipController::getNearestFlag( void )
{
    auto pos = mStarShip->getPosition( );
    float nearestDistance = 0.0f;
    unsigned int nearestFlag = 0;
    for( unsigned int i = 5; i < (unsigned int)ARENA->BLUETEAM->flags.size( ); ++i )
    {
        auto flag = ARENA->BLUETEAM->flags[i];
        if( flag->isCapturable( ) )
        {
            sf::Vector2f targetPos = flag->getPosition( );
            pos.x = targetPos.x - pos.x;
            pos.y = targetPos.y - pos.y;
            pos.x *= pos.x;
            pos.y *= pos.y;
            float distance = std::sqrt( pos.y + pos.x );
            if( nearestDistance == 0.0f )
            {
                nearestDistance = std::abs( distance );
                nearestFlag = i;
            }
            else if( std::abs( distance ) < std::abs( nearestDistance ) ) {
                nearestDistance = std::abs( distance );
                nearestFlag = i;
            }
        }
    }
    return nearestFlag;
}
