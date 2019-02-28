#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    Player        		= 1 << 0,
    Wall          		= 1 << 1,
    NPC            		= 1 << 2,
    Item				= 1 << 3,
    Warp                = 1 << 4,
    ImageLayer          = 1 << 5,
    TileLayer           = 1 << 6,
    ObjectLayer         = 1 << 7,
    ParticleSystem      = 1 << 8,
    ParticleLayer		= 1 << 9,
    SoundEffect			= 1 << 10
};

}
#endif // CATEGORY_HPP
