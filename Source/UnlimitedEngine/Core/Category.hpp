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
    Red3                = 1 << 4,
    Blue1   			= 1 << 5,
    Blue2               = 1 << 6,
    Blue3               = 1 << 7,
    ImageLayer          = 1 << 8,
    GoalRed             = 1 << 9,
    ObjectLayer         = 1 << 10,
    ParticleSystem      = 1 << 11,
    ParticleLayer		= 1 << 12,
    SoundEffect			= 1 << 13,
    BumperBlue          = 1 << 14,
    BumperRed           = 1 << 15,
    GoalBlue            = 1 << 16,
    Bullet              = 1 << 17,
    Network             = 1 << 18,
    FlagRed             = 1 << 19,
    FlagBlue            = 1 << 20,
    BlueTeam = Player  | Blue1 | Blue2 | Blue3,
    RedTeam =  Player2 | Red1  | Red2  | Red3
};

}
#endif // CATEGORY_HPP
