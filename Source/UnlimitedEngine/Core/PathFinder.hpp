#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <iostream> // For debugging
#include <vector>
#include <set>
#include <math.h>

#include <SFML/System/Vector2.hpp>
#include "Game/Grid.hpp"

template <class T>
class PathFinder
{
public:
    PathFinder(std::vector<T> grid, T* start, T* goal, std::function<float(T* start, T* goal)> heuristic );
    sf::Vector2i getNextMove( void );
    unsigned int size( void ) { return mPath.size(); }


    std::vector<sf::Vector2i> mPath;

private:
    std::vector<sf::Vector2i> calculatePath(std::vector<T> grid, T* start, T* goal , std::function<float(T *, T *)> heuristic);
    ///
    /// \brief isValid
    /// Will be used someday to block the unit from going to impossible Ts such as walls, rivers, and the edge of te map.
    /// \param pos
    /// the T being tested to make sure it can be moved to
    /// \return
    /// flag if the T is valid
    bool isValid( T* ) { return true; }
    ///
    /// \brief isGoal
    /// Flags whether the T being worked on is the T we are looking for
    /// (algorithm complete) or if it should keep looking.
    /// \param current
    /// \param goal
    /// \return
    ///
    bool isGoal( T* current, T* goal ) { return ( current == goal ); }
    ///
    /// \brief checkOpenList
    /// searches openList and returns true if the current param is on the list
    /// \param current
    /// \param openList
    /// \return
    ///
    bool checkOpenList( T* current, std::set<T*> openList );
    ///
    /// \brief checkClosedList
    /// searches closed list and returns true if the current param is on the list
    /// \param current
    /// \param closedList
    /// \return
    ///
    bool checkClosedList( T* current, std::set<T*> closedList );

};

#include "Core/PathFinder.inl"
#endif // PATHFINDER_HPP
