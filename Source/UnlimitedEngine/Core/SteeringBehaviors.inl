#include "SteeringBehaviors.hpp"


template<class T>
void SteeringBehaviors::wander( T& obj )
{

}

template<class T, class K>
void SteeringBehaviors::arrive( T& obj, sf::Vector2f targetPos )
{

}

template<class T>
void SteeringBehaviors::seek( T& obj, sf::Vector2f targetPos )
{
    float targetAngle = bearing( obj->getPosition( ).x, obj->getPosition( ).y, targetPos.x, targetPos.y );
    float rot = obj->getRotation( );

    if( 180 > rot )
        if( 180 > targetAngle )
            if( rot < targetAngle - obj->speed( ) )
                obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )
                obj->rotate( -obj->speed( ) );
            else
                obj->thrust( );
        else  // 180 < target
            if( (360 - targetAngle) + rot < 180 )
                obj->rotate(-obj->speed() );
            else if( 360 - targetAngle + rot > 180 )
                obj->rotate( obj->speed() );
            else
                obj->thrust( );
    else // 180 < rot
        if( 180 > targetAngle )
            if( (360 - targetAngle) + rot < 180 )
                obj->rotate( -obj->speed( ) );
            else if( (360 - targetAngle) + rot > 180 )
                obj->rotate( obj->speed( ) );
            else
                obj->thrust( );
        else  // 180 < target
            if( rot < targetAngle - obj->speed( ) )
                obj->rotate( obj->speed( ) );
            else if( rot > targetAngle + obj->speed( ) )
                obj->rotate( -obj->speed( ) );
            else
                obj->thrust( );

}

template<class T, class K>
void SteeringBehaviors::pursuit( T& obj, K& target )
{

}

template<class T, class K>
void SteeringBehaviors::evade( T& obj, K& target )
{

}
