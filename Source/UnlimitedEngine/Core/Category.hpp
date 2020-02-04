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
    RedUnit             = 1 << 9,
    BlueUnit            = 1 << 10,
    RedBuilding         = 1 << 11,
    BlueBuilding        = 1 << 12,
    NeutralBuilding     = 1 << 13,
    Red                 = RedUnit | RedBuilding,
    Blue                = BlueUnit | BlueBuilding,
};

}
#endif // CATEGORY_HPP
