#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    Player        		= 1 << 0,
    Player2       		= 1 << 1,
    ImageLayer          = 1 << 2,
    ObjectLayer         = 1 << 3,
    ParticleSystem      = 1 << 4,
    ParticleLayer		= 1 << 5,
    SoundEffect			= 1 << 6,
    Network             = 1 << 7,
    TileLayer           = 1 << 8,

};

}
#endif // CATEGORY_HPP
