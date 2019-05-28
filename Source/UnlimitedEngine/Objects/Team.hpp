#ifndef TEAM_HPP
#define TEAM_HPP

#include <array>
#include <vector>

#include "Objects/StarShip.hpp"
#include "Objects/Flag.hpp"
#include "Objects/Bumper.hpp"
#include "Objects/Goal.hpp"

struct Team {
    Team( Category::Type type )
        : flags( std::vector<Flag*>() )
    {
        if( type == Category::BlueTeam )
        {
            goal = new Goal( Category::GoalBlue );
            bumper = new Bumper( Category::BumperBlue );
        }
        else if( type == Category::RedTeam )
        {
            goal = new Goal( Category::GoalRed );
            bumper = new Bumper( Category::BumperRed );
        }
        else
            assert( "Team Constructor ERROR" == std::string("") );
    }
    ~Team( void )
    {
        delete bumper;
        delete goal;
    }
    std::array<StarShip*, 3> starShips;
    Bumper* bumper;
    Goal* goal;
    std::vector<Flag*> flags;
};


#endif // TEAM_HPP
