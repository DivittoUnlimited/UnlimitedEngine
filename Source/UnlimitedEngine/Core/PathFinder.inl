#include "Core/PathFinder.hpp"
#include "Core/Globals.hpp"

template<class T>
PathFinder<T>::PathFinder(std::vector<T> grid, T* start, T* goal, std::function<float(T *, T *)> heuristic )
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
std::vector<sf::Vector2i> PathFinder<T>::calculatePath( std::vector<T> grid, T* start, T* goal, std::function<float(T*,T*)> heuristic )
{   
    //goal->gridIndex = sf::Vector2i( goal->gridIndex.y, goal->gridIndex.x );
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
                node->heuristicCost = heuristic( node, start );

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

    // no path can be made becuase it is unreachable, or the user is already there.
    if( !isValid( goal ) || *start == goal ) return path;

    // add the starting element to the open list to start the algorithm
    goal->parent = nullptr;
    openList.insert( goal );

    // run loop untill the goal has been found or you have run out of options to check (invalid path)
      while( ( goal->gridIndex != start->gridIndex ) && !openList.empty() ) // DOES THIS EVEN MAKE SENSE???????????????????????????//
      {
          closedList.insert( goal );
          if( goal->gridIndex.y * 16 + goal->gridIndex.x > 0 && static_cast<unsigned int>( goal->gridIndex.y * 16 + goal->gridIndex.x) < grid.size( ) )
          {
              if( goal->gridIndex.y - 1 > 0 )
                calcT( goal, &grid.at( ( goal->gridIndex.y - 1 ) * 16 + goal->gridIndex.x ) );
              if( goal->gridIndex.y + 1 < 12 )
                calcT( goal, &grid.at( ( goal->gridIndex.y + 1 ) * 16 + goal->gridIndex.x ) );
              if( goal->gridIndex.x - 1 > 0 )
                calcT( goal, &grid.at( goal->gridIndex.y * 16 + ( goal->gridIndex.x - 1 ) ) );
              if( goal->gridIndex.x + 1 < 16 )
                calcT( goal, &grid.at( goal->gridIndex.y * 16 + ( goal->gridIndex.x + 1 ) ) );
          }
          T* cheapest = (*openList.begin());
          for( auto i : openList )
          {
              if( (*i) < cheapest )
                  cheapest = i;
          }
          // this doesn't return the node with the lowest cost
          goal = *openList.find( cheapest );

          openList.erase( cheapest );
      }
      // build path here by reversing nodes taken to get to the goal
      if( goal->gridIndex == start->gridIndex )
      {
          // path found just have to save it
          while( goal->parent != nullptr )
          {
              goal = goal->parent;
              path.push_back( sf::Vector2i( goal->gridIndex.x, goal->gridIndex.y ) );
              std::cout << "Pos: " << goal->gridIndex.x << ", " << goal->gridIndex.y << " Pos Cost: " << goal->costSoFar + goal->heuristicCost << std::endl;
          }
          std::cout << "======================" << std::endl;
      }
      std::vector<sf::Vector2i> final;
      while( path.size() > 0 )
      {
          final.push_back( path.back() );
          path.pop_back();
      }
      return final;
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
