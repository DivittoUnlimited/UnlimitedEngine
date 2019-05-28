#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    Player        		= 1 << 0,
    Player2       		= 1 << 1,
    PlayerShip          = 1 << 2,
    StarShipBlue  		= 1 << 3,
    FlagBlue			= 1 << 4,
    FlagRed             = 1 << 5,
    ImageLayer          = 1 << 6,
    GoalRed             = 1 << 7,
    ObjectLayer         = 1 << 8,
    ParticleSystem      = 1 << 9,
    ParticleLayer		= 1 << 10,
    SoundEffect			= 1 << 11,
    StarShipRed         = 1 << 12,
    BumperBlue          = 1 << 13,
    BumperRed           = 1 << 14,
    GoalBlue            = 1 << 15,
    Bullet              = 1 << 16,
    Network             = 1 << 17,
    RedTeam             = 1 << 18,
    BlueTeam            = 1 << 19
};

}
#endif // CATEGORY_HPP
