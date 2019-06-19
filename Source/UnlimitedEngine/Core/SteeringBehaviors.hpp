#ifndef STEERINGBEHAVIORS_HPP
#define STEERINGBEHAVIORS_HPP

#include <math.h>
#include <SFML/System/Vector2.hpp>

class SteeringBehaviors
{
public:
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


    SteeringBehaviors( void ) { }

    template<class T>
    static void wander( T& obj );

    template<class T, class K>
    static void arrive( T& obj, sf::Vector2f targetPos );

    template<class T>
    static void seek( T& obj, sf::Vector2f targetPos );

    template<class T, class K>
    static void pursuit( T& obj, K& target );

    template<class T, class K>
    static void evade( T& obj, K& target );

};

#include "SteeringBehaviors.inl"

#endif // STEERINGBEHAVIORS_HPP
