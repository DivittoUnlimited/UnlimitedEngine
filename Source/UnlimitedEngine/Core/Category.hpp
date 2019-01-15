#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    ImageLayer  		= 1 << 0,
    Player      		= 1 << 1,

    TileLayer           = 1 << 2,
    ObjectLayer         = 1 << 3,
    NPC            		= 1 << 4,
    Item				= 1 << 5,
    Trigger             = 1 << 6,
    ParticleSystem		= 1 << 7,
    SoundEffect			= 1 << 8
};

}
#endif // CATEGORY_HPP
