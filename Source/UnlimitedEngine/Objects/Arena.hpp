#ifndef ARENA_HPP
#define ARENA_HPP

#include "Team.hpp"

struct Arena {
    Arena( void )
        : REDTEAM( new Team( Category::RedTeam ) )
        , BLUETEAM( new Team( Category::BlueTeam ) )
    { }

    Team* REDTEAM;
    Team* BLUETEAM;
    // Score board
    // sound effects and other assets...
    // Penalty Box
    // Power up Comets
};

#endif // ARENA_HPP
