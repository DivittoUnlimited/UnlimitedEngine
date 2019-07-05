#include "SteeringBehaviors.hpp"
#include <iostream>

template<class T>
void SteeringBehaviors::wander( T& obj )
{

}

template<class T>
void SteeringBehaviors::seek( T obj, sf::Vector2f targetPos )
{
    assert( obj );
    float targetAngle = 0.0f;
    float rot = obj->getRotation( );

    // Solve so AI can use Screen wrap effect
    if( obj->getPosition( ).x > targetPos.x && ( ( WINDOW_WIDTH - obj->getPosition( ).x ) + targetPos.x < obj->getPosition( ).x - targetPos.x ) ) // if distance to target is shorter using wrap, owner to the right
        targetAngle = bearing( -obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );
    else if( obj->getPosition( ).x < targetPos.x && ( ( WINDOW_WIDTH - targetPos.x ) + obj->getPosition( ).x < targetPos.x - obj->getPosition( ).x ) ) // if distance to target is shorter using wrap, owner to the left
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, -targetPos.x, targetPos.y );
    else // ships closer not using wrap effect, solve normally
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );

    if( 180 >= rot ) {
        if( 180 > targetAngle ) {
            if( rot < targetAngle - obj->speed( ) )         obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )    obj->rotate( -obj->speed( ) );
            else                                            obj->thrust( obj->speed( ) );
        }
        else {  // 180 < target
            if( (360 - targetAngle) + rot < 180 )           obj->rotate(-obj->speed( ) );
            else if( 360 - targetAngle + rot > 180 )        obj->rotate( obj->speed( ) );
            else                                            obj->thrust( obj->speed( ) );
        }
    }
     else {
        if( 180 > targetAngle ) {
            if( (360 - targetAngle) + rot < 180 )           obj->rotate( -obj->speed( ) );
            else if( (360 - targetAngle) + rot > 180 )      obj->rotate( obj->speed( ) );
            else                                            obj->thrust( obj->speed( ) );
        }
        else {  // 180 < target
            if( rot < targetAngle - obj->speed( ) )         obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )    obj->rotate( -obj->speed( ) );
            else                                            obj->thrust( obj->speed( ) );
        }
    }
}

template<class T>
void SteeringBehaviors::evade( T obj, sf::Vector2f targetPos )
{
    assert( obj );
    float targetAngle = 0.0f;
    float rot = obj->getRotation( );

    // Solve so AI can use Screen wrap effect
    if( obj->getPosition( ).x > targetPos.x && ( ( WINDOW_WIDTH - obj->getPosition( ).x ) + targetPos.x < obj->getPosition( ).x - targetPos.x ) ) // if distance to target is shorter using wrap, owner to the right
        targetAngle = bearing( -obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );
    else if( obj->getPosition( ).x < targetPos.x && ( ( WINDOW_WIDTH - targetPos.x ) + obj->getPosition( ).x < targetPos.x - obj->getPosition( ).x ) ) // if distance to target is shorter using wrap, owner to the left
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, -targetPos.x, targetPos.y );
    else // ships closer not using wrap effect, solve normally
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );

    if( targetAngle < 180 )
        targetAngle += 180;
    else if( targetAngle >= 180 )
        targetAngle -= 180;

    if( 180 >= rot )
    {
        if( 180 >= targetAngle )
        {
            if( rot < targetAngle - obj->speed( ) )
                obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )
                obj->rotate( -obj->speed( ) );
            else {
                obj->thrust( obj->speed( ) );
            }
        }else { // 180 < target
            if( (360 - targetAngle) + rot < 180 )
                obj->rotate( -obj->speed( ) );
            else if( (360 - targetAngle) + rot > 180 )
                obj->rotate( obj->speed( ) );
            else {
                obj->thrust( obj->speed( ) );
            }
        }
    }
    else // 180 < rot
    {
        if( 180 >= targetAngle )
        {
            if( (360 - targetAngle) + rot < 180 )
                obj->rotate( obj->speed( ) );
            else if( (360 - targetAngle) + rot > 180 )
                obj->rotate( -obj->speed(  ) );
            else
                obj->thrust( obj->speed( ) );
        }else if( 180 < targetAngle )
        {
            if( rot < targetAngle - obj->speed( ) )
                obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )
                obj->rotate( -obj->speed( ) );
            else
                obj->thrust( obj->speed( ) );
        }
    }
}

template<class T>
void SteeringBehaviors::arrive( T obj, sf::Vector2f targetPos )
{
    assert( obj );
    float targetAngle = 0.0f;
    float rot = obj->getRotation( );

    // Solve so AI can use Screen wrap effect
    if( obj->getPosition( ).x > targetPos.x && ( ( WINDOW_WIDTH - obj->getPosition( ).x ) + targetPos.x < obj->getPosition( ).x - targetPos.x ) ) // if distance to target is shorter using wrap, owner to the right
        targetAngle = bearing( -obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );
    else if( obj->getPosition( ).x < targetPos.x && ( ( WINDOW_WIDTH - targetPos.x ) + obj->getPosition( ).x < targetPos.x - obj->getPosition( ).x ) ) // if distance to target is shorter using wrap, owner to the left
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, -targetPos.x, targetPos.y );
    else // ships closer not using wrap effect, solve normally
        targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );

    // solve distance to target
    auto pos = obj->getPosition( );
    pos.x = targetPos.x - pos.x;
    pos.y = targetPos.y - pos.y;
    pos.x *= pos.x;
    pos.y *= pos.y;

    if( std::abs( std::sqrt( pos.y + pos.x ) ) < 100 )
    {
        evade( obj, targetPos );
    }
    else
    {
        bool thrustFlag = true;
        if( 180 >= rot ) {
            if( 180 > targetAngle ) {
                if( rot < targetAngle - obj->speed( ) ){
                    obj->rotate( obj->speed( ) );
                    thrustFlag = false;
                }
                else if( rot > targetAngle + obj->speed( ) ) {
                    obj->rotate( -obj->speed( ) );
                    thrustFlag = false;
                }
            }
            else {  // 180 < target
                if( (360 - targetAngle) + rot < 180 ) {
                    obj->rotate(-obj->speed( ) );
                    thrustFlag = false;
                }
                else if( 360 - targetAngle + rot > 180 ) {
                    obj->rotate( obj->speed( ) );
                    thrustFlag = false;
                }
            }
        }
         else {
            if( 180 > targetAngle ) {
                if( (360 - targetAngle) + rot < 180 ) {
                    obj->rotate( -obj->speed( ) );
                    thrustFlag = false;
                }
                else if( (360 - targetAngle) + rot > 180 ) {
                    obj->rotate( obj->speed( ) );
                    thrustFlag = false;
                }
            }
            else {  // 180 < target
                if( rot < targetAngle - obj->speed( ) ) {
                    obj->rotate( obj->speed( ) );
                    thrustFlag = false;
                }
                else if( rot > targetAngle + obj->speed( ) ) {
                    obj->rotate( -obj->speed( ) );
                    thrustFlag = false;
                }
            }
        }
        if( thrustFlag ) {
            obj->thrust( obj->speed( ) );
        }
    }
}
