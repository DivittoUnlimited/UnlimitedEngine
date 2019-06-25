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

    // Activate needed command to "seek" to target.
    if( rot <= targetAngle + obj->speed( ) && rot >= targetAngle - obj->speed( ) ) // obj facing target edge case
        obj->thrust( );
    else {
        if( 180 >= rot ) {
            if( 180 > targetAngle ) {
                if( rot < targetAngle - obj->speed( ) )      obj->rotate( obj->speed( ) );
                else if( rot > targetAngle + obj->speed( ) ) obj->rotate( -obj->speed( ) );
                else                                         obj->thrust( );
            }
            else {  // 180 < target
                if( (360 - targetAngle) + rot < 180 )    obj->rotate(-obj->speed( ) );
                else if( 360 - targetAngle + rot > 180 ) obj->rotate( obj->speed( ) );
                else                                     obj->thrust( );
            }
        }
        else {
            if( 180 > targetAngle ) {
                if( (360 - targetAngle) + rot < 180 )      obj->rotate( -obj->speed( ) );
                else if( (360 - targetAngle) + rot > 180 ) obj->rotate( obj->speed( ) );
                else                                       obj->thrust( );
            }
            else {  // 180 < target
                if( rot < targetAngle - obj->speed( ) )      obj->rotate( obj->speed( ) );
                else if( rot > targetAngle + obj->speed( ) ) obj->rotate( -obj->speed( ) );
                else                                         obj->thrust( );
            }
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
                obj->thrust( );
            }
        }else { // 180 < target
            if( (360 - targetAngle) + rot < 180 )
                obj->rotate( -obj->speed( ) );
            else if( (360 - targetAngle) + rot > 180 )
                obj->rotate( obj->speed( ) );
            else {
                obj->thrust( );
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
                obj->thrust( );
        }else if( 180 < targetAngle )
        {
            if( rot < targetAngle - obj->speed( ) )
                obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )
                obj->rotate( -obj->speed( ) );
            else
                obj->thrust( );
        }
    }
}
