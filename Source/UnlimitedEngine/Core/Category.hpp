#ifndef CATEGORY_HPP
#define CATEGORY_HPP

// Entity/scene node category, used to dispatch commands
namespace Category {
enum Type
{
    None				= 0,
    Player        		= 1 << 0,
    Player2       		= 1 << 1,
    Red1                = 1 << 2,
    Red2          		= 1 << 3,
    Blue1   			= 1 << 4,
    Blue2               = 1 << 5,
    ImageLayer          = 1 << 6,
    GoalRed             = 1 << 7,
    ObjectLayer         = 1 << 8,
    ParticleSystem      = 1 << 9,
    ParticleLayer		= 1 << 10,
    SoundEffect			= 1 << 11,
    BumperBlue          = 1 << 12,
    BumperRed           = 1 << 13,
    GoalBlue            = 1 << 14,
    Bullet              = 1 << 15,
    Network             = 1 << 16,
    FlagRed             = 1 << 17,
    FlagBlue            = 1 << 18,
    BlueTeam = Player  | Blue1 | Blue2,
    RedTeam =  Player2 | Red1  | Red2
};

}
#endif // CATEGORY_HPP
