#include "Core/PathFinder.hpp"
#include "Core/Globals.hpp"

template<class T>
PathFinder<T>::PathFinder(std::vector<std::vector<T>> grid, T* start, T* goal, std::function<float(T *, T *)> heuristic )
{
   mPath = calculatePath( grid, start, goal, heuristic );
}

template<class T>
sf::Vector2i PathFinder<T>::getNextMove( void )
{
    auto temp = mPath.back( );
    mPath.pop_back( );
    return temp;
}

template<class T>
std::vector<sf::Vector2i> PathFinder<T>::calculatePath( std::vector<std::vector<T>> grid, T* start, T* goal, std::function<float(T*,T*)> heuristic )
{
    std::vector<sf::Vector2i> path;
    std::set<T*> openList;
    std::set<T*> closedList;

    // auto calcT = [&openList, &closedList, this, goal]( T* current, T* node ) -> void {
    auto calcT = [&]( T* current, T* node ) -> void {
        if( isValid( node ) && !checkClosedList( node, closedList ) )
        {
            if( !checkOpenList( node, openList ) )
            {
                node->parent = current;
                node->costSoFar = current->costSoFar + heuristic( current, node );
                node->heuristicCost = heuristic( node, goal );
                openList.insert( node );

                //std::cout << "New Node added to possibles: " << node->gridIndex.x << ", " << node->gridIndex.y << std::endl;
            }
            else // node already on the open list.
            {
                if( node->costSoFar > current->costSoFar + heuristic( current, node ) )
                {
                    openList.erase( node );
                    node->parent = current;
                    node->costSoFar = current->costSoFar + heuristic( current, node );
                    openList.insert( node );
                    //std::cout << "Node has been READDED: " << node->gridIndex.x << ", " << node->gridIndex.y << std::endl;
                }
            }
        }
    };

    if( !isValid( goal ) || *start == goal ) return path;

    // add the starting element to the open list to start the algorithm
    start->parent = nullptr;
    openList.insert( start );

    // run loop untill the goal has been found or you have run out of options to check (invalid path)
    while( ( start->gridIndex != goal->gridIndex ) && !openList.empty() ) // DOES THIS EVEN MAKE SENSE???????????????????????????//
    {
        closedList.insert( start );
        if( start->gridIndex.y >= 0 && static_cast<unsigned int>( start->gridIndex.y) < grid.at( start->gridIndex.x ).size( ))
        {
            if( (start->gridIndex.x > 0 ) )
                calcT( start, &grid.at( start->gridIndex.x - 1 ).at( start->gridIndex.y ) );
            if( static_cast<unsigned int>(start->gridIndex.x + 1) < grid.size( ) )
                calcT( start, &grid.at( start->gridIndex.x + 1 ).at( start->gridIndex.y ) );
        }
        if( (start->gridIndex.x >= 0 && static_cast<unsigned int>(start->gridIndex.x) < grid.size( ) ) )
        {
            if( start->gridIndex.y > 0 )
                calcT( start, &grid.at( start->gridIndex.x ).at( start->gridIndex.y - 1 ) );
            if( static_cast<unsigned int>( start->gridIndex.y + 1) < grid.at( start->gridIndex.x ).size( ) )
                calcT( start, &grid.at( start->gridIndex.x ).at( start->gridIndex.y + 1 ) );
        }

        T* cheapest = (*openList.begin());
        for( auto i : openList )
        {
            if( (*i) < cheapest )
                *cheapest = *i;
        }
        // this doesn't return the node with the lowest cost
        start = *openList.find( cheapest );

        openList.erase( cheapest );
    }
    // build path here by reversing nodes taken to get to the goal
    if( start->gridIndex == goal->gridIndex )
    {
        // path found just have to save it
        while( start->parent != nullptr )
        {
            path.push_back( sf::Vector2i( start->gridIndex.x, start->gridIndex.y ) );
            //std::cout << "Pos: " << start->gridIndex.y << ", " << start->gridIndex.x << " has been added to the PATH" << std::endl;
            start = start->parent;
        }

    }
    return path;
}

template<class T>
bool PathFinder<T>::checkOpenList( T* current, std::set<T*> openList )
{
    for( auto node : openList )
        if( current == node )
            return true;
    return false;
}

template<class T>
bool PathFinder<T>::checkClosedList( T* current, std::set<T*> closedList )
{
    for( auto node : closedList )
        if( current == node )
            return true;
    return false;
}
