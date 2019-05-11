#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    Player        		= 1 << 0,
    GoalA          		= 1 << 1,
    StarShipA      		= 1 << 2,
    FlagA				= 1 << 3,
    FlagB               = 1 << 4,
    ImageLayer          = 1 << 5,
    GoalB               = 1 << 6,
    ObjectLayer         = 1 << 7,
    ParticleSystem      = 1 << 8,
    ParticleLayer		= 1 << 9,
    SoundEffect			= 1 << 10,
    StarShipB           = 1 << 11
};

}
#endif // CATEGORY_HPP
